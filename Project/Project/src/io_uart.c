/*
 * io_uart.c
 *
 * This file contains functions for UART communication.
 * 
 * Created by: Mathias Beckius, 2014-03-18
 */ 

#include "io_uart.h"

/* UART Control Register (pointer) */
reg_address_t *const p_UART_CR = (reg_address_t *) 0x400E0800U;
/* UART Mode Register (pointer) */
reg_address_t *const p_UART_MR = (reg_address_t *) 0x400E0804U;
/* UART Status Register (pointer) */
reg_address_t *const p_UART_SR = (reg_address_t *) 0x400E0814U;
/* UART Receiver Holding Register (pointer) */
reg_address_t *const p_UART_RHR = (reg_address_t *) 0x400E0818U;
/* UART Transmit Holding Register (pointer) */
reg_address_t *const p_UART_THR = (reg_address_t *) 0x400E081CU;
/* UART Baud Rate Generator Register (pointer) */
reg_address_t *const p_UART_BRGR = (reg_address_t *) 0x400E0820U;

/*
 * UART Configuration
 * Configures UART communication with a certain baud rate.
 */
void uart_config(uint32_t baud)
{
	/* reset and disable receiver & transmitter */
	UART_CR = UART_CR_RSTRX | UART_CR_RSTTX	| UART_CR_RXDIS | UART_CR_TXDIS;
	/* configure baud rate */
	UART_BRGR = (MCK >> 4) / baud;
	/* configure mode */
	UART_MR = UART_MR_PAR_NO | UART_MR_CHMODE_NORMAL;
	/* enable receiver and transmitter */
	UART_CR = UART_CR_RXEN | UART_CR_TXEN;
	/* configure RX0 pin as pull-up */
	ioport_set_pin_mode(PIO_PA8_IDX, IOPORT_MODE_PULLUP);	
}

/*
 * Transmitter Ready?
 * Return 1 if "Transmitter Ready" flag is set, otherwise 0.
 */
int uart_transmitter_ready(void)
{
	return (UART_SR & UART_SR_TXRDY);
}

/*
 * Receiver Ready?
 * Return 1 if "Receiver Ready" flag is set, otherwise 0.
 */
int uart_receiver_ready(void)
{
	return (UART_SR & UART_SR_RXRDY);
}

/*
 * Send character
 * Write character to Transmit Holding Register.
 */
void uart_send_char(uint8_t chr)
{
	UART_THR = chr;
}

/*
 * Receive character
 * Read character from Receiver Holding Register.
 */
char uart_receive_char(void)
{
	char chr = UART_RHR;
	return chr;
}