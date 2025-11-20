/*
 * TimerConf.h
 *
 *  Created on: Jul 22, 2025
 *      Author: Gabriel
 */

#ifndef TIMERCONF_H_
#define TIMERCONF_H_

#include "stm32f4xx.h"
#include <stdint.h>


void Timer2_PWM_Init(void);
void Timer2_PWM_CA_Init(void);
void Timer2_SetDutyCycle(uint16_t duty_percent);

#endif /* TIMERCONF_H_ */
