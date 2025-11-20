// This task is a UART test/loopback demo on STM32f4 board
//UART2 is connected to your laptop (through USB)
//UART1 is treated like a second device the STM32F4 can talk to
// The program sends whatever you type to the second UART
#include "uart_driver.h"        // Custom UART driver
#include "systick_config.h"		//Systick timer  setup used by UART timeouts
uint8_t uart1_success = 0; // for debugging
UART_Status_t UART1_TX;         // Variable to hold UART1 transmit status

int main(void){
	// Initialise the SyStick timer
	// The UART driver uses this for timeouts and delays

	SysTick_Init();

	// Configure UART1: 115200 baud, 8 data bits, 1 stop bits, no parity
	UART_Config_t uart1_config = {
			.baudrate = 115200,
			.parity = UART_PARITY_NONE,
			.stop_bits = UART_STOPBITS_1,
			.word_length = UART_WORDLENGTH_8B
	};
	//Configure UART2 with the same settings
	UART_Config_t uart2_config = {
			.baudrate = 115200,
			.word_length = UART_WORDLENGTH_8B,
			.stop_bits = UART_STOPBITS_1,
			.parity = UART_PARITY_NONE,
	};
	// initialise UART2 and run test if OK
	if (UART1_Init(&uart1_config) == UART_OK){
		uart1_success = 1;
	}

	// Initialise UART2
	if (UART2_Init(&uart2_config) == UART_OK) {
		// Send a test message
		uint8_t msg[] = "Hello, UART2!\r\n";
		UART2_Transmit(msg, sizeof(msg) - 1, 1000);

		// Send a test string to the PC terminal over UART2
		// Echo received data
		uint8_t received_byte;

		// Main loop: echo data between UART2 and UART1
		while (1) {
			// Check if any data has been received on UART2
			if (UART2_IsDataAvailable()) {
				//  Read one byte from UART2(from PC)
				received_byte = UART2_ReceiveByte();
				// Send that byte out on UART1 and save the status result
				UART1_TX = UART1_TransmitByte(received_byte); // Echo back/ forward to UART1

				// Send the UART1 transmit status back to the PC over UART2
				UART2_TransmitByte((uint8_t)UART1_TX); // Echo UART1 TX result
			}
		}
	}

}
