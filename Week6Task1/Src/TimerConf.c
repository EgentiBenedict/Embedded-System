#include "TimerConf.h"

void Timer2_PWM_Init(void){
	// Enable GPIOA and TIM2 clocks
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

	// Set PA0 output and to alternate function mode
	GPIOA->MODER &= ~GPIO_MODER_MODE0_Msk;
	GPIOA->MODER |= GPIO_MODER_MODE0_1;

	GPIOA->AFR[0] &= ~(GPIO_AFRL_AFRL0);
	GPIOA->AFR[0] |= GPIO_AFRL_AFRL0_0;

	/* Configure TIM2 for PWM */

	//Variables for clocks, prescaler and period
	// timer_clock = 48000000;
	uint32_t prescaler = 48-1; // 48E6/48 = 1E6
	uint32_t pwmFreq = 1000-1; // 1E6/1E3 = 1E3, 1 kHz

	// Configure prescaler and auto-reload register

	TIM2->PSC = prescaler;
	TIM2->ARR = pwmFreq;

	// Set PWM mode 1 on channel 1
	TIM2->CCMR1 &= ~TIM_CCMR1_OC1M;
	TIM2->CCMR1 |= (0b110 << TIM_CCMR1_OC1M_Pos);
	TIM2->CCMR1 |= TIM_CCMR1_OC1PE;

	// Enable output on TIM2_CH1
	TIM2->CCER |= TIM_CCER_CC1E;
	// Auto-reload preload enable
	TIM2->CR1 |= TIM_CR1_ARPE;
	//Generate update event
	TIM2->EGR |= TIM_EGR_UG;
	// Enable TIM2 counter
	TIM2->CR1 |= TIM_CR1_CEN;
}

void Timer2_PWM_CA_Init(void){
	// Enable GPIOA and TIM2 clocks
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;

	// Set PA0 output and to alternate function mode
	GPIOA->MODER &= ~GPIO_MODER_MODE0_Msk;
	GPIOA->MODER |= GPIO_MODER_MODE0_1;

	GPIOA->AFR[0] &= ~(GPIO_AFRL_AFRL0);
	GPIOA->AFR[0] |= GPIO_AFRL_AFRL0_0;

	/* Configure TIM2 for PWM */

	//Variables for clocks, prescaler and period
	// timer_clock = 48000000;
	uint32_t prescaler = 48-1; // 48E6/48 = 1E6
	uint32_t pwmFreq = 1000-1; // 1E6/(2*1E3) = 500 Hz

	// Configure prescaler and auto-reload register

	TIM2->PSC = prescaler;
	TIM2->ARR = pwmFreq;

	// Set PWM mode 1 on channel 1
	TIM2->CCMR1 &= ~TIM_CCMR1_OC1M;
	TIM2->CCMR1 |= (0b110 << TIM_CCMR1_OC1M_Pos);
	TIM2->CCMR1 |= TIM_CCMR1_OC1PE;

	// Enable output on TIM2_CH1
	TIM2->CCER |= TIM_CCER_CC1E;
	// Auto-reload preload enable
	TIM2->CR1 |= TIM_CR1_ARPE;

	// Set center-aligned mode 1 (CMS = 01)
    TIM2->CR1 &= ~TIM_CR1_CMS;
    TIM2->CR1 |= (0b01 << TIM_CR1_CMS_Pos);

	//Generate update event
	TIM2->EGR |= TIM_EGR_UG;
	// Enable TIM2 counter
	TIM2->CR1 |= TIM_CR1_CEN;
}


void Timer2_SetDutyCycle(uint16_t duty_percent){
	if (duty_percent > 100) duty_percent = 100;
	TIM2->CCR1 = (TIM2->ARR + 1)*duty_percent/100;
}

void Timer2_SetDutyCycle_CentreAligned(uint16_t duty_percent){

	// This function should be used for CentreAligned modes 1 and 2.

	uint32_t arr = TIM2->ARR;
	uint32_t ccr_value = 0;

	ccr_value = (2*(arr + 1) * duty_percent) / 100;
	if(ccr_value > arr) ccr_value = arr;


	TIM2->CCR1 = ccr_value;
}
