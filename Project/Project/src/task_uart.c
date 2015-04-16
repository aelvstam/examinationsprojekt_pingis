/*
 * task_uart.c
 *
 * Created: 2015-03-18 14:24:44
 *  Author: Andreas & Peter
 */ 
 #include <stdlib.h>
 #include <FreeRTOS.h>
 #include <FreeRTOSConfig.h>
 #include <asf.h>
 #include "task.h"
 #include "io_uart.h"
 #include "semaphore.h"
 #include "io_pwm.h"
 #include "task_uart.h"
 
 uint16_t invalue;
 uint8_t timer;
 uint16_t outvalue;
 int16_t error;
 uint8_t multiplier;
 uint8_t input;
 
 /******************************************************************************/
 /* Task for handling the communication to Matlab through serial communication */
 /******************************************************************************/
 void task_uart(void *pvParameters)
 {
	/* Pointer to a variable that holds the time at which the task was last unblocked.
	The variable must be initialized with the current time prior to its first use.
	Following this the variable is automatically updated within vTaskDelayUntil() */
	portTickType xLastWakeTime; 
	
	 /* The cycle time period. The task will be unblocked at time (*pxPreviousWakeTime + xTimeIncrement).
	Calling vTaskDelayUntil with the same xTimeIncrement parameter value will cause the task to execute with a fixed interval period. */
	const portTickType xTimeIncrement = timer; //50-100ms
	
	xLastWakeTime = xTaskGetTickCount(); /* Initialize the xLastWakeTime variable with the current time. */
	
	//task loops runs infinitively 
	while (1)
	{
		if(uart_receive_char() == 6)											/* Checking for MATLAB */
		{
			if(xSemaphoreTake(semafor_signal, portMAX_DELAY))
			{
				set_correct_bits(invalue);
				
				while(!uart_transmitter_ready());
				uart_send_char(multiplier);
				uart_send_char(terminator);
				
				while(!uart_transmitter_ready());
				uart_send_char(input);											
				uart_send_char(terminator);
				
				set_correct_bits(outvalue);
				
				while(!uart_transmitter_ready());
				uart_send_char(multiplier);
				uart_send_char(terminator);
				
				while(!uart_transmitter_ready());
				uart_send_char(input);
				uart_send_char(terminator);
				
				set_correct_bits(error);
				
				while(!uart_transmitter_ready());
				uart_send_char(multiplier);
				uart_send_char(terminator);
				
				while(!uart_transmitter_ready());
				uart_send_char(input);
				uart_send_char(terminator);
													
				xSemaphoreGive(semafor_signal);
			}	
		}
		vTaskDelayUntil( &xLastWakeTime, xTimeIncrement);
	}
	 
 }
 
 /************************************************************************/
 /* Method for setting the task timer                                    */
 /************************************************************************/
 void set_communication_time(uint8_t dt)
 {
	 timer = dt;
 }
 
 /************************************************************************/
 /* Method for setting the invalue from the sensor                       */
 /************************************************************************/
 void set_invalue(uint16_t value)
 {
	 invalue = value;
 }
 /************************************************************************/
 /* Method for setting the outvalue of the PWM-signal                    */
 /************************************************************************/
 void set_outvalue(uint16_t value)
 {
	 outvalue = value;
 }
 /************************************************************************/
 /* Method for setting the error value                                   */
 /************************************************************************/
 void set_error(int16_t value)
 {
	 error = value;
 }
 
 /************************************************************************/
 /* Method for positive or negative 10-bit calculation for 8-bit sending */
 /************************************************************************/
 void set_correct_bits(int16_t value)
 {
	 if((value > 767) && (value < 1024))
	 {
		multiplier = 3;
		input = (value - 768);
	 }
	 if((value > 511) && (value < 768))
	 {
		 multiplier = 2;
		 input = (value - 512);
	 }
	 if((value > 255) && (value < 512))
	 {
		 multiplier = 1;
		 input = (value - 256);
	 }
	 if((value >= 0) && (value < 256))
	 {
		multiplier = 0;
		input = value;
	 }
	 if((value < 0) && (value > -256))
	 {
		 multiplier = 5;
		 input = -value;
	 }
	 if((value < -255) && (value > -512))
	 {
		 multiplier = 6;
		 input = -(value + 256);
	 }
	 if((value < -511) && (value > -768))
	 {
		 multiplier = 7;
		 input = -(value + 512);
	 }
	 if((value < -767) && (value > -1024))
	 {
		multiplier = 8; 
		input = -(value + 768);
	 }
 }