#include "uart_driver.h"
#include "stm32f4xx.h"
#include <stdbool.h>
#include "systick_config.h" // for GetTick();

#define UART1_TIMEOUT_MS 1000

// ====================== UART DRIVER ======================

// GPIO Configuration for USART1
// PA15 - TX (AF7)
// PB7  - RX (AF7)

UART_Status_t UART1_Init(UART_Config_t* config) {
    // Enable SysTick if not already running

    // Enable clocks
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN;   // GPIOA/B clock
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;  // USART1 clock

    // Configure GPIO pins PA15 (TX) and PB7 (RX)
    GPIOA->MODER &= ~GPIO_MODER_MODER15;
    GPIOA->MODER |= GPIO_MODER_MODER15_1;

    GPIOB->MODER &= ~GPIO_MODER_MODER7;
	GPIOB->MODER |= GPIO_MODER_MODER7_1;

	// Fast speed switching for TX and RX
    GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR15_1;
    GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR7_1;

    GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR15);
    GPIOB->PUPDR |= GPIO_PUPDR_PUPDR7_0;  // Pull-up on RX

    // Alternate function configuration for PA15
    GPIOA->AFR[1] &= ~GPIO_AFRH_AFSEL15_Msk;
    GPIOA->AFR[1] |= (7 << GPIO_AFRH_AFSEL15_Pos);

    // Alternate function configuration for PB7
    GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL7_Msk;
    GPIOB->AFR[0] |= (7 << GPIO_AFRL_AFSEL7_Pos);

    // Reset USART1
    USART1->CR1 = 0;
    USART1->CR2 = 0;
    USART1->CR3 = 0;

    // Configure USART1
    USART1->CR1 |= config->word_length | config->parity;
    USART1->CR2 |= config->stop_bits;

    // Set baud rate
    if (UART1_SetBaudRate(config->baudrate) != UART_OK) {
        return UART_ERROR;
    }

    // Enable USART1, transmitter and receiver
    USART1->CR1 |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;

    return UART_OK;
}


UART_Status_t UART1_DeInit(void) {
    USART1->CR1 &= ~USART_CR1_UE;
    RCC->APB2ENR &= ~RCC_APB2ENR_USART1EN;
    GPIOA->MODER &= ~GPIO_MODER_MODER15;
    GPIOB->MODER &= ~GPIO_MODER_MODER7;
    return UART_OK;
}


UART_Status_t UART1_SetBaudRate(uint32_t baudrate) {
    uint32_t pclk = 16000000; // APB1 at 16 MHz default
    uint32_t tmp  = (pclk + baudrate/2) / baudrate;

    if (tmp < 16 || tmp > 0xFFFF) {
        return UART_ERROR;
    }
    USART1->BRR = tmp;
    return UART_OK;
}


UART_Status_t UART1_Transmit(uint8_t* data, uint16_t size, uint32_t timeout) {
    uint32_t start_tick = GetTick();

    for (uint16_t i = 0; i < size; i++) {
        while (!(USART1->SR & USART_SR_TXE)) {
            if ((GetTick() - start_tick) > timeout) {
                return UART_TIMEOUT;
            }
        }
        USART1->DR = data[i];
    }

    while (!(USART1->SR & USART_SR_TC)) {
        if ((GetTick() - start_tick) > timeout) {
            return UART_TIMEOUT;
        }
    }

    return UART_OK;
}


UART_Status_t UART1_Receive(uint8_t* data, uint16_t size, uint32_t timeout) {
    uint32_t start_tick = GetTick();

    for (uint16_t i = 0; i < size; i++) {
        while (!(USART1->SR & USART_SR_RXNE)) {
            if ((GetTick() - start_tick) > timeout) {
                return UART_TIMEOUT;
            }
        }
        data[i] = (uint8_t)(USART1->DR & 0xFF);
    }
    return UART_OK;
}


UART_Status_t UART1_TransmitByte(uint8_t data) {
    uint32_t start_tick = GetTick();

    while (!(USART1->SR & USART_SR_TXE)) {
        if ((GetTick() - start_tick) > UART1_TIMEOUT_MS) {
            return UART_TIMEOUT;
        }
    }
    USART1->DR = data;
    return UART_OK;
}


uint8_t UART1_ReceiveByte(void) {
    while (!(USART1->SR & USART_SR_RXNE));
    return (uint8_t)(USART1->DR & 0xFF);
}


bool UART1_IsDataAvailable(void) {
    return (USART1->SR & USART_SR_RXNE) != 0;
}

bool UART1_IsTransmitComplete(void) {
    return (USART1->SR & USART_SR_TC) != 0;
}
