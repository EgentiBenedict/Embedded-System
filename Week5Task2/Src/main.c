//This program measures the period of the square wave, it calculates the difference between edges
//and measures the frequency of a square wave signal
#include "stm32f4xx.h"

//Timing state variables
volatile uint32_t tim3_overflow = 0;
volatile uint32_t last_capture = 0;
volatile uint32_t period_ticks = 0;
volatile float period_ms = 0;     

//forward declaration of helper used by ISR
void TIM3_IC_callback(void);

//Configure TIM3 channel 4 for input capture
void inputReceived_TIM3CH4(void)
{
	//Enable GPIOC and TIM3  peripheral clocks
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;

    //Set PC9 to alternate function mode
    GPIOC->MODER &= ~GPIO_MODER_MODER9;    
    GPIOC->MODER |= GPIO_MODER_MODER9_1;

    //Select AF2 for PC9
    GPIOC->AFR[1] &= ~GPIO_AFRH_AFSEL9;      
    GPIOC->AFR[1] |= 4 << GPIO_AFRH_AFSEL9_Pos;

    //Timer runs at base clock
    TIM3->PSC = 1 - 1;
    ///Use full 16-bit range for the counter
    TIM3->ARR = 0xFFFF;

    //Configure channel 4 as input, mapped to T14
    TIM3->CCMR2 |= TIM_CCMR2_CC4S_0;

    //Capture on active edge, no inversion
    TIM3->CCER &= ~(TIM_CCER_CC4P | TIM_CCER_CC4NP);

    //Enable capture on channel 4
    TIM3->CCER |= TIM_CCER_CC4E;

    //Enable interrupt
    TIM3->DIER |= TIM_DIER_UIE | TIM_DIER_CC4IE;

    //Force an update event so prescaler are loaded
    TIM3->EGR |= TIM_EGR_UG;

    //start the timer
    TIM3->CR1 |= TIM_CR1_CEN;

    //Allow TIM3 interrupts in the NVIc
    NVIC_EnableIRQ(TIM3_IRQn);
}

int main(void) {
    while(1) {
    	//This helps the main loop do other tasks
        inputReceived_TIM3CH4();
    };
}

//T~IM3 global interrupt handler
void TIM3_IRQHandler(void) {

	//update interrupt
    if (TIM3->SR & TIM_SR_UIF) {
        tim3_overflow++;
        TIM3->SR &= ~TIM_SR_UIF; 
    }

    //Capture event on channel 4
    if (TIM3->SR & TIM_SR_CC4IF) {
        TIM3_IC_callback();      
        TIM3->SR &= ~TIM_SR_CC4IF;
    }
}
//This helps each input capture event to compute the signal period
void TIM3_IC_callback(void) {
    uint32_t capture = TIM3->CCR4;                  
    uint32_t timestamp = (tim3_overflow << 16) | capture; 

    //Number of ticks since last edge
    period_ticks = timestamp - last_capture;

    //Save for next measurement
    last_capture = timestamp;

    //Convert tricks to milliseconds
    period_ms = (float)period_ticks / 16000.0;
}
