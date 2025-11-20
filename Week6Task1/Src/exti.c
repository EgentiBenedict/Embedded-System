#include "exti.h"

void pc13_exti_init(void){

	// Disable global interrupts

	__disable_irq();

	// Enable clock access to GPIO C

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

	// Set PC13 as Input

	GPIOC->MODER &= ~GPIO_MODER_MODER13_Msk;

	// Enable clock access to SYSCFG

	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	// Select PORTC for EXTI13

	SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI13;
	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;


	// Unmask EXTI13

	EXTI->IMR |= EXTI_IMR_IM13;

	// Select falling edge trigger

	EXTI->FTSR |= EXTI_FTSR_TR13;

	// Enable EXTI line in NVIC

	NVIC_EnableIRQ(EXTI15_10_IRQn);

	// Enable global interrupts

	__enable_irq();
}
