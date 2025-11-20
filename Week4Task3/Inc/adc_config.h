/*
 * adc_config.h
 *
 *  Created on: Oct 20, 2025
 *      Author: Nekki
 */

#ifndef ADC_CONFIG_H_
#define ADC_CONFIG_H_

#include "stm32f4xx.h"
#include <stdint.h>
void adc_PA_init(uint8_t pin);
uint16_t adc_PA_read(void);

#endif /* ADC_CONFIG_H_ */
