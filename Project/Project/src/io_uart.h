/*
 * io_uart.h
 *
 * This file contains functions for UART communication.
 *
 * Created by: Mathias Beckius, 2014-03-18
 */ 

#ifndef IO_UART_H_
#define IO_UART_H_

#include <asf.h>

typedef uint32_t reg_address_t;

/* Master Clock Frequency (84 MHz) */
#define MCK 84000000U

/* UART Control Register */
#define UART_CR (*p_UART_CR)
/* UART Mode Register */
#define UART_MR (*p_UART_MR)
/* UART Status Register */
#define UART_SR (*p_UART_SR)
/* UART Receiver Holding Register */
#define UART_RHR (*p_UART_RHR)
/* UART Transmit Holding Register */
#define UART_THR (*p_UART_THR)
/* UART Baud Rate Generator Register */
#define UART_BRGR (*p_UART_BRGR)

/* UART Configuration */
void uart_config(uint32_t);
/* Transmitter Ready? */
int uart_transmitter_ready(void);
/* Receiver Ready? */
int uart_receiver_ready(void);
/* Send character */
void uart_send_char(uint8_t);
/* Receive character */ 
char uart_receive_char(void);

#endif