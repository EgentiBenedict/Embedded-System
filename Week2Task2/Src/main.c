//This program is an LED chaser that changes direction depending on an input pin
//If the switch is high then LEDs move in forward sequence
//If the switch is LOW the LEDs move backward in sequence

#include "stm32f4xx.h"

int main(void){

	//Enable clock to GPIOC
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

	//Set GPIOC PC5, PC6, PC8, PC9 as output
	GPIOC->MODER &= ~(GPIO_MODER_MODER5_Msk);
	GPIOC->MODER |= GPIO_MODER_MODER5_0;

	GPIOC->MODER &= ~(GPIO_MODER_MODER6_Msk);
	GPIOC->MODER |= GPIO_MODER_MODER6_0;

	GPIOC->MODER &= ~(GPIO_MODER_MODER8_Msk);
	GPIOC->MODER |= GPIO_MODER_MODER8_0;

	GPIOC->MODER &= ~(GPIO_MODER_MODER9_Msk);
	GPIOC->MODER |= GPIO_MODER_MODER9_0;

	//Set PC10 as input for the switch/jumper
	GPIOC->MODER &= ~GPIO_MODER_MODER10_Msk;

	//Infinite loop. If PC10 is high = forward, else = reverse
	while(1){
		//LED moves forward direction 9->8->6->5
		if (GPIOC->IDR & GPIO_IDR_ID10){
			GPIOC->BSRR |= (1 << 9);
			for(int i = 0; i < 300000; i++);
			GPIOC->BSRR |= (1 << (9+16));

			GPIOC->BSRR |= (1 << 8);
			for(int i = 0; i < 300000; i++);
			GPIOC->BSRR |= (1 << (8+16));

			GPIOC->BSRR |= (1 << 6);
			for(int i = 0; i < 300000; i++);
			GPIOC->BSRR |= (1 << (6+16));

			GPIOC->BSRR |= (1 << 5);
			for(int i = 0; i < 300000; i++);
			GPIOC->BSRR |= (1 << (5+16));
			}
		else{
			//LED moves in the reverse direction 5->6->8->9
			GPIOC->BSRR |= (1 << 5);
			for(int i = 0; i < 300000; i++);
			GPIOC->BSRR |= (1 << (5+16));

			GPIOC->BSRR |= (1 << 6);
			for(int i = 0; i < 300000; i++);
			GPIOC->BSRR |= (1 << (6+16));

			GPIOC->BSRR |= (1 << 8);
			for(int i = 0; i < 300000; i++);
			GPIOC->BSRR |= (1 << (8+16));

			GPIOC->BSRR |= (1 << 9);
			for(int i = 0; i < 300000; i++);
			GPIOC->BSRR |= (1 << (9+16));

		}

	}

}
