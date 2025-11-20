#include "uart_driver.h"
#include "stm32f4xx.h"
#include <stdbool.h>
#include "systick_config.h"

// Timeout value (in ms)
#define UART2_TIMEOUT_MS 1000

// ====================== UART DRIVER ======================

// GPIO Configuration for USART2
// PA2 - TX (AF7)
// PA3 - RX (AF7)

UART_Status_t UART2_Init(UART_Config_t* config) {
    // Enable SysTick if not already running

    // Enable clocks
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;   // GPIOA clock
    RCC->APB1ENR |= RCC_APB1ENR_USART2EN;  // USART2 clock

    // Configure GPIO pins PA2 (TX) and PA3 (RX)
    GPIOA->MODER &= ~(GPIO_MODER_MODER2 | GPIO_MODER_MODER3);
    GPIOA->MODER |= (GPIO_MODER_MODER2_1 | GPIO_MODER_MODER3_1);

    GPIOA->OSPEEDR |= (GPIO_OSPEEDER_OSPEEDR2 | GPIO_OSPEEDER_OSPEEDR3);

    GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPDR2 | GPIO_PUPDR_PUPDR3);
    GPIOA->PUPDR |= GPIO_PUPDR_PUPDR3_0;  // Pull-up on RX

    GPIOA->AFR[0] &= ~(GPIO_AFRL_AFRL2 | GPIO_AFRL_AFRL3);
    GPIOA->AFR[0] |= (7 << GPIO_AFRL_AFSEL2_Pos) | (7 << GPIO_AFRL_AFSEL3_Pos);

    // Reset USART2
    USART2->CR1 = 0;
    USART2->CR2 = 0;
    USART2->CR3 = 0;

    // Configure USART2
    USART2->CR1 |= config->word_length | config->parity;
    USART2->CR2 |= config->stop_bits;

    // Set baud rate
    if (UART2_SetBaudRate(config->baudrate) != UART_OK) {
        return UART_ERROR;
    }

    // Enable USART2, transmitter and receiver
    USART2->CR1 |= USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;

    return UART_OK;
}


UART_Status_t UART2_DeInit(void) {
    USART2->CR1 &= ~USART_CR1_UE;
    RCC->APB1ENR &= ~RCC_APB1ENR_USART2EN;
    GPIOA->MODER &= ~(GPIO_MODER_MODER2 | GPIO_MODER_MODER3);
    return UART_OK;
}


UART_Status_t UART2_SetBaudRate(uint32_t baudrate) {
    uint32_t pclk = 16000000; // APB1 at 16 MHz default
    uint32_t tmp  = (pclk + baudrate/2) / baudrate;

    if (tmp < 16 || tmp > 0xFFFF) {
        return UART_ERROR;
    }
    USART2->BRR = tmp;
    return UART_OK;
}


UART_Status_t UART2_Transmit(uint8_t* data, uint16_t size, uint32_t timeout) {
    uint32_t start_tick = GetTick();

    for (uint16_t i = 0; i < size; i++) {
        while (!(USART2->SR & USART_SR_TXE)) {
            if ((GetTick() - start_tick) > timeout) {
                return UART_TIMEOUT;
            }
        }
        USART2->DR = data[i];
    }

    while (!(USART2->SR & USART_SR_TC)) {
        if ((GetTick() - start_tick) > timeout) {
            return UART_TIMEOUT;
        }
    }

    return UART_OK;
}


UART_Status_t UART2_Receive(uint8_t* data, uint16_t size, uint32_t timeout) {
    uint32_t start_tick = GetTick();

    for (uint16_t i = 0; i < size; i++) {
        while (!(USART2->SR & USART_SR_RXNE)) {
            if ((GetTick() - start_tick) > timeout) {
                return UART_TIMEOUT;
            }
        }
        data[i] = (uint8_t)(USART2->DR & 0xFF);
    }
    return UART_OK;
}


UART_Status_t UART2_TransmitByte(uint8_t data) {
    uint32_t start_tick = GetTick();

    while (!(USART2->SR & USART_SR_TXE)) {
        if ((GetTick() - start_tick) > UART2_TIMEOUT_MS) {
            return UART_TIMEOUT;
        }
    }
    USART2->DR = data;
    return UART_OK;
}


uint8_t UART2_ReceiveByte(void) {
    while (!(USART2->SR & USART_SR_RXNE));
    return (uint8_t)(USART2->DR & 0xFF);
}


bool UART2_IsDataAvailable(void) {
    return (USART2->SR & USART_SR_RXNE) != 0;
}

bool UART2_IsTransmitComplete(void) {
    return (USART2->SR & USART_SR_TC) != 0;
}
