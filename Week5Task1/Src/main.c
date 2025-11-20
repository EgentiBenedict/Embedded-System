/*This task configures the STM32's onboard LED(PA5) as a digital output
and repeatedly toggles it every second using a SysTick-based delay.*/


//Define device
#include "stm32f4xx.h"

//SysTick delay function
#include "systick_config.h"

int main(void){

	//Enable the clock to GPIOA peripheral
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	//Set PA5 as output
	//Clear the bits first, then set them
	GPIO->MODER &= ~(GPIO_MODER_MODER5_Msk);
	GPIO->MODER |= GPIO_MODER_MODER5_0;

	//Infinite loop to blink the LED
	while(1){
		//Toggle the LED
		GPIO->ODR ^= GPIO_ODR_OD5;

		//Wait 1000ms
		delayMs(1000);
	}
}
