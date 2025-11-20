// This STM32F4 program uses PA4 as an analogue input for an ADC, reads the analogue
//voltage, converts it into a digital value, and then calculates the corresponding
// voltage and temperature.
#include "stm32f4xx.h"    //STM32 hardware definitions
#include "adc_config.h"  //header file for ADC setup and functions

uint16_t adc_val;       //Variable to store ADC raw digital value
float voltage;          //Variable to store converted voltage
float temp;             //Variable to store calculated temperature

int main(void){

	// Initialises PA4 as ADC input channel
	adc_PA_init(4);

	// Sets PA5 as output (clock already enable in adc_PA_init)
	GPIOA->MODER &= ~GPIO_MODER_MODER5_Msk;
	GPIOA->MODER |= GPIO_MODER_MODER5_0;

	while(1){
	//Read the ADC value from PA4 (0-4095 for 12-bit ADC)
		adc_val = adc_PA_read();
	//Converts ADC value to actual voltage
		voltage = 3.3 * adc_val/4095;
	//Converts voltage to temperature
		temp = (voltage-0.5)*100.0;

//		if (adc_val < 2048){
//			GPIOA->BSRR |= GPIO_BSRR_BR5;
//		} else{
//			GPIOA->BSRR |= GPIO_BSRR_BS5;
//		}
	 //delay loop to slow down readings
		for(int i = 0; i < 400000; i++);
	}
}


