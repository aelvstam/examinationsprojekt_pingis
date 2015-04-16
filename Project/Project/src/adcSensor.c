/*
 * adc.c
 *
 * Created: 2015-03-19 09:50:42
 *  Author: Andreas & Peter
 */ 

#include <asf.h>
#include "adcSensor.h"
#include "math.h"
#include "stdio.h"

/************************************************************************/
/* Setup for the ADC                                                    */
/************************************************************************/
void adc_setup()
{
	pmc_enable_periph_clk(ID_ADC);
	adc_init(ADC, sysclk_get_main_hz(), 20000000, 0);		// Initialize ADC 
	adc_configure_timing(ADC, 0, 0, 0);
	adc_set_resolution(ADC,ADC_MR_LOWRES);
	adc_enable_channel(ADC, ADC_CHANNEL_10);				// Enable PB17 - AD8 for Arduino Due
	adc_configure_trigger(ADC, ADC_TRIG_SW, 0);				// Setup trigger by software
}
