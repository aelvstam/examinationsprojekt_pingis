

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */

#include <asf.h>
#include "io_uart.h"
#include "task_uart.h"
#include "task_pid.h"
#include "semaphore.h"
#include "adcSensor.h"
#include "io_pwm.h"

xSemaphoreHandle semafor_signal = 1;

/************************************************************************/
/* Initialization of the system                                         */
/************************************************************************/
int main (void) 
{
	/* initialization of SAM system */
	sysclk_init();
	board_init();
	ioport_init();
	
	/* Setup the motor shield */
	ioport_set_pin_dir(CHECK_PIN, IOPORT_DIR_INPUT);		//input on pin 22
	ioport_set_pin_dir(PIO_PC22_IDX,IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(PIO_PC22_IDX,LOW);
	ioport_set_pin_dir(PIO_PB27_IDX,IOPORT_DIR_OUTPUT);
	ioport_set_pin_level(PIO_PB27_IDX,HIGH);
	
	/* Creates the binary semaphore */
	vSemaphoreCreateBinary(semafor_signal);
	
	adc_setup();
	pwm_setup();											/* Setup the PWM*/
	
	/* Configure UART */
	uint32_t baud_rate = 115200;
	uart_config(baud_rate);
	matlab_configure();
	
	/* Create first task for serial communication to MATLAB*/
	if (xTaskCreate(task_uart, (const signed char * const) "Uart",TASK_UART_STACK_SIZE, NULL, TASK_UART_STACK_PRIORITY, NULL) !=pdPASS)
	{
				//should never end up here
	}
	
	/* Create second task for PID regulation*/
	if (xTaskCreate(task_pid, (const signed char * const) "PID",TASK_PID_STACK_SIZE, NULL, TASK_PID_STACK_PRIORITY, NULL) !=pdPASS)
	{
				//should never end up here
	}
	
	/* Starts the FreeRTOS scheduler running all tasks indefinitely*/
	vTaskStartScheduler();
}
