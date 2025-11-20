#ifndef UART_DRIVER_H_
#define UART_DRIVER_H_

#include "stm32f4xx.h"
#include <stdint.h>
#include <stdbool.h>

// ================= UART CONFIGURATION =================
typedef struct {
    uint32_t baudrate;
    uint32_t word_length;    // USART_CR1_M (0 = 8 bits, 1 = 9 bits)
    uint32_t stop_bits;      // USART_CR2_STOP (00 = 1 bit, 10 = 2 bits)
    uint32_t parity;         // USART_CR1_PCE and USART_CR1_PS
} UART_Config_t;

// Predefined configurations
#define UART_WORDLENGTH_8B     0x00000000U
#define UART_WORDLENGTH_9B     USART_CR1_M

#define UART_STOPBITS_1        0x00000000U
#define UART_STOPBITS_2        USART_CR2_STOP_1

#define UART_PARITY_NONE       0x00000000U
#define UART_PARITY_EVEN       USART_CR1_PCE
#define UART_PARITY_ODD        (USART_CR1_PCE | USART_CR1_PS)

// ================= UART STATUS =================
typedef enum {
    UART_OK = 0,
    UART_ERROR,
    UART_BUSY,
    UART_TIMEOUT
} UART_Status_t;

// ================= UART1 DRIVER =================
UART_Status_t UART1_Init(UART_Config_t* config);
UART_Status_t UART1_DeInit(void);
UART_Status_t UART1_SetBaudRate(uint32_t baudrate);
UART_Status_t UART1_Transmit(uint8_t* data, uint16_t size, uint32_t timeout_ms);
UART_Status_t UART1_Receive(uint8_t* data, uint16_t size, uint32_t timeout_ms);
UART_Status_t UART1_TransmitByte(uint8_t data);
uint8_t       UART1_ReceiveByte(void);
bool          UART1_IsDataAvailable(void);
bool          UART1_IsTransmitComplete(void);

// ================= UART2 DRIVER =================
UART_Status_t UART2_Init(UART_Config_t* config);
UART_Status_t UART2_DeInit(void);
UART_Status_t UART2_SetBaudRate(uint32_t baudrate);
UART_Status_t UART2_Transmit(uint8_t* data, uint16_t size, uint32_t timeout_ms);
UART_Status_t UART2_Receive(uint8_t* data, uint16_t size, uint32_t timeout_ms);
UART_Status_t UART2_TransmitByte(uint8_t data);
uint8_t       UART2_ReceiveByte(void);
bool          UART2_IsDataAvailable(void);
bool          UART2_IsTransmitComplete(void);

#endif /* UART_DRIVER_H_ */
