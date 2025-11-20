/*
 * adc.config.c
 *
 *  Created on: Oct 20, 2025
 *      Author: Nekki
 */


#include "adc_config.h"

void adc_PA_init(uint8_t pin){
    /*
     * Configures the desired GPIOA pin capable of ADC.
     * PA0 to PA7 -> Channel 0 to Channel 7
     */

    // Enable clock to GPIOA and ADC1
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

    // Set GPIOA pin to analogue mode
    GPIOA->MODER |= (0b11 << (2*pin)); // Set to analog mode (11)
    GPIOA->PUPDR &= ~(0b11 << (2*pin)); // No pull-up/pull-down

    // Ensure ADC is off before configuration
    ADC1->CR2 &= ~ADC_CR2_ADON;

    // Small delay to ensure ADC is fully powered down
    for(int i = 0; i < 1000; i++);

    // Resolution to 12 bits (00 = 12-bit)
    ADC1->CR1 &= ~ADC_CR1_RES;

    // Set conversion sequence length to 1 conversion
    ADC1->SQR1 &= ~ADC_SQR1_L; // L[3:0] = 0000 for 1 conversion

    // Choose channel for first conversion in sequence
    ADC1->SQR3 &= ~ADC_SQR3_SQ1; // Clear SQ1 bits
    ADC1->SQR3 |= (pin << ADC_SQR3_SQ1_Pos); // Set channel

    // Set sampling time based on the pin/channel
    // With 16MHz ADC clock, 480 cycles = 30Î¼s sampling time (plenty for most cases)
    // You could reduce to 84 cycles (5.25Î¼s) or 56 cycles (3.5Î¼s) for faster conversions

	uint32_t smpr2_pos = pin * 3; // Each channel uses 3 bits
	ADC1->SMPR2 &= ~(0b111 << smpr2_pos); // Clear existing bits
	ADC1->SMPR2 |= (0b111 << smpr2_pos);  // 480 cycles sampling time


    // Enable ADC
    ADC1->CR2 |= ADC_CR2_ADON;

    // Wait for ADC to stabilize (recommended delay)
    for(int i = 0; i < 1000; i++);
}

uint16_t adc_PA_read(void){
    // Start conversion
    ADC1->CR2 |= ADC_CR2_SWSTART;

    // Wait for conversion to complete
    while(!(ADC1->SR & ADC_SR_EOC));

    // Read and return the result (12-bit mask)
    return (uint16_t)(ADC1->DR & 0x0FFF);
}
