/*
 * io_pwm.c
 *
 * Created: 2015-03-31 12:05:14
 *  Author: Andreas & Peter
 */ 
#include "io_pwm.h"
#include <pio.h>
#include <sysclk.h>

static pwm_channel_t pwm_channel_instance;

/************************************************************************/
/* Initializing PWM.                                                    */
/* Returns 1 if setup was successful, otherwise 0 is returned.          */
/************************************************************************/

int pwm_setup(void)
{
	/* Enable the specified peripheral clock (PWM clock).
	   If function returns 0, then we can proceed... */
	if (pmc_enable_periph_clk(ID_PWM) == 0) {
		pwm_channel_disable(PWM, PWM_CHANNEL);
		pwm_clock_t clock_setting = {
			.ul_clka = PWM_FREQUENCY * PWM_RESOLUTION,
			.ul_clkb = 0,
			.ul_mck = sysclk_get_cpu_hz()
		};
		pwm_init(PWM, &clock_setting);
		pwm_channel_instance.alignment = PWM_ALIGN_LEFT;
		pwm_channel_instance.polarity = PWM_POLARITY;
		pwm_channel_instance.ul_prescaler = PWM_CMR_CPRE_CLKA;
		pwm_channel_instance.ul_period = PWM_RESOLUTION;
		pwm_channel_instance.ul_duty = 0;
		pwm_channel_instance.channel = PWM_CHANNEL;
		pwm_channel_init(PWM, &pwm_channel_instance);
		/* setup PWM for pin */
		pio_set_peripheral(PWM_PIO, PWM_PIO_PERIPHERAL, PWM_PIO_PIN);
		/* enable PWM channel */
		pwm_channel_enable(PWM, PWM_CHANNEL);
		/* indicate everything's OK! */
		return 1;
	}
	/* indicate that something went wrong! */	
	return 0;
}

/************************************************************************/
/* Set duty cycle of PWM signal                                         */
/************************************************************************/
void pwm_set_duty_cycle(uint32_t duty_cycle)
{
	pwm_channel_update_duty(PWM, &pwm_channel_instance, duty_cycle);
}