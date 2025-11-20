// This task wires a push button on PC13 to an external interrupt, and uses
// that interrupt to step a PWM duty cycle (on TIM2) by 25% each press, wrapping
// back to 0% after 100%.


#include "exti.h"           //EXTI init prototypes
#include "myClockConf.h"   //System clock configuration prototypes
#include "TimerConf.h"	   //SetDutyCycle prototypes
#include "stm32f4xx.h"

#define UBTN EXTI_PR_PR13  //mask for EXTI line 13 pending bit

uint32_t duty = 0;


int main(void){

	setClock();			   //Configure system clock (PLL, buses, etc.)
	pc13_exti_init();      //Configure PC13 + EXTI13 + NVIC
	Timer2_PWM_Init();	   //Configure TIMER2 PWM (period + channel + enable)

/*	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	GPIOA->MODER &= GPIOA_MODER_MODER5_Msk;
	GPIOA->MODER |= GPIOA_MODER_MODER5_0;*/

	//main loop does nothing; all work happens in the interrupt
	while(1){}
}
//This is the interrupt handler for EXTI lines 10 to 15.
//The Nucleo board's user button is connected to PC13,
//so pressing it triggers EXTI and this handler is executed.
void EXTI15_10_IRQHandler(void){
	if(EXTI->PR & UBTN)     //Step duty cycle by 25% on each press
	{
		duty += 25;
				if(duty > 100){
					duty = 0;   //wrap back to 0%
				}
		//Apply new duty cycle to TIMER2 PWM
		Timer2_SetDutyCycle(duty);
	}
	//Clear EXTI13, allowing it to detect the next button being pressed
	EXTI->PR |= UBTN;
}

