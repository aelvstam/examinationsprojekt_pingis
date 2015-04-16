/*
 * task_uart.h
 *
 * Created: 2015-03-18 14:26:43
 *  Author: Andreas & Peter
 */ 


#ifndef TASK_UART_H_
#define TASK_UART_H_

#define TASK_UART_STACK_SIZE (2048/sizeof(portSTACK_TYPE))
#define TASK_UART_STACK_PRIORITY (tskIDLE_PRIORITY)
#define terminator 0xA

void task_uart(void *pvParameters);

void set_communication_time(uint8_t);

void set_invalue(uint16_t);

void set_outvalue(uint16_t);

void set_error(int16_t);

void matlab_communication(void);

void set_correct_bits(int16_t);

#endif /* TASK_UART_H_ */