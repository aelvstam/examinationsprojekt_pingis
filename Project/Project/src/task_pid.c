/*
 * task_pid.c
 *
 * Created: 2015-03-18 16:27:41
 *  Author: Andreas & Peter
 */ 

#include <stdlib.h>
#include <FreeRTOS.h>
#include <FreeRTOSConfig.h>
#include <asf.h>
#include <math.h>
#include "task.h"
#include "task_pid.h"
#include "semaphore.h"
#include "adcSensor.h"
#include "io_uart.h"
#include "task_uart.h"
#include "io_pwm.h"

uint16_t setPoint;
uint8_t timer;

float proportionalGain;
float timeIntegral;
float timeDerivate;
float dT;

/******************************************************************************/
/* Task for regulating the PWM signal to the fan attached to the motor shield */
/******************************************************************************/
void task_pid(void *pvParameters)
{
	/* Pointer to a variable that holds the time at which the task was last unblocked.
	The variable must be initialized with the current time prior to its first use.
	Following this the variable is automatically updated within vTaskDelayUntil() */
	portTickType xLastWakeTime; 
	/* The cycle time period. The task will be unblocked at time (*pxPreviousWakeTime + xTimeIncrement).
	Calling vTaskDelayUntil with the same xTimeIncrement parameter value will cause the task to execute with a fixed interval period. */
	const portTickType xTimeIncrement = timer; // timer is configured from Matlab en received by serial communication
	
	xLastWakeTime = xTaskGetTickCount(); /* Initialize the xLastWakeTime variable with the current time. */
	
	//task loop runs infinitively 
	while (1)
	{
		regulator();
		vTaskDelayUntil( &xLastWakeTime, xTimeIncrement);
	}
}

/************************************************************************/
/* Method that uses a PID-regulator and a FIR-filter                    */
/************************************************************************/
void regulator()
{	
	static float xbuff[M+1] = {0};
	static float b[M+1] = {0.25,0.25,0.25,0.25};
	static float integral = 0;
	static int16_t prevD = 0;
	float derivate;
	float temp = 0;
	int k;
	uint16_t invalue;
	uint16_t pwmValue;
	int16_t error;
	
	adc_start(ADC);
	while((adc_get_status(ADC) & 0x1<<24)==0);		//Wait until DRDY get high
	invalue=(adc_get_latest_value(ADC));
	
	for(k = M-1;k >= 0;k--)
	{
		xbuff[k+1] = xbuff[k];
	}
	xbuff[0] = invalue;
	
	for(k = 0;k <= M;k++)
	{
		temp += b[k]*xbuff[k];
	}
	invalue = (uint16_t)temp;
	
	error = (setPoint - invalue);
	integral = (integral + (error * dT));
	derivate = ((error - prevD) / dT);
	prevD = error;
	pwmValue = (proportionalGain * (error + (timeDerivate * derivate) + (integral / timeIntegral)));
	if(pwmValue > 999)
	{
		pwmValue = 999;
	}
	if (pwmValue < 0)
	{
		pwmValue = 0;
	}
	pwm_set_duty_cycle(pwmValue);
	if(xSemaphoreTake(semafor_signal, portMAX_DELAY))
	{
		set_invalue(invalue);
		set_outvalue(pwmValue);
		set_error(error);
		xSemaphoreGive(semafor_signal);
	}
}

/***************************************************************/
/* Method for receiving all the variables in the PID regulator */
/***************************************************************/
void matlab_configure()
{
	uint8_t multiplier;
	uint8_t nbr;
	uint8_t kp;
	uint8_t td;
	uint8_t ti;
	
	while(uart_receive_char() != 5);						/* Waiting for MATLAB */
	while(!uart_receiver_ready());
	multiplier = uart_receive_char();
	
	while(!uart_receiver_ready());
	nbr = uart_receive_char();
	setPoint = ((256 * multiplier) + nbr);
	
	while(!uart_receiver_ready());
	multiplier = uart_receive_char();
	
	while(!uart_receiver_ready());
	kp = uart_receive_char();
	proportionalGain = ((float)kp/(pow(10, multiplier)));
	
	while(!uart_receiver_ready());
	multiplier = uart_receive_char();
	
	while(!uart_receiver_ready());
	ti = uart_receive_char();
	timeIntegral = ((float)ti/(pow(10, multiplier)));
	
	while(!uart_receiver_ready());
	multiplier = uart_receive_char();
	
	while(!uart_receiver_ready());
	td = uart_receive_char();
	timeDerivate = ((float)td/(pow(10, multiplier)));
	
	while(!uart_receiver_ready());
	timer = uart_receive_char();
	set_communication_time(timer);
	dT = ((float)timer/1000);
}
