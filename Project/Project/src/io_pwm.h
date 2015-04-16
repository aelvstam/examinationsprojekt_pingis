/*
 * io_pwm.h
 *
 * Created: 2015-03-31 12:07:25
 *  Author: Andreas & Peter
 */ 


#ifndef IO_PWM_H_
#define IO_PWM_H_

#include <pwm.h>

/* Frequency of PWM signal */
#define PWM_FREQUENCY		1000
/* Resolution: 1000 steps between 0-100% duty cycle */
#define PWM_RESOLUTION		999
/* PWM channel */
#define PWM_CHANNEL			PWM_CHANNEL_0
/* Parallel Input/Output Controller for pin using PWM output */
#define PWM_PIO				PIOB
/* Peripheral that is controlling the pin */
#define PWM_PIO_PERIPHERAL	PIO_PERIPH_B
/* Output pin for PWM signal: PB16 (DAC1 on the Arduino Due) */
#define PWM_PIO_PIN			PIO_PB16B_PWML0	/* low output */
/* Low polarity of PWM signal.
   It's recommended to match low polarity with low output. This means that a
   higher value for the duty cycle will result in a higher duty cycle,
   otherwise it's inverted. */
#define PWM_POLARITY		PWM_LOW

/* Initialize PWM */
int pwm_setup(void);
/* Set duty cycle of PWM signal */
void pwm_set_duty_cycle(uint32_t);

#endif /* IO_PWM_H_ */