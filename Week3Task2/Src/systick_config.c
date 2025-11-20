#ifndef SYSTICK_CONFIG_H_
#define SYSTICK_CONFIG_H_

#include "stm32f4xx.h"
#include <stdint.h>

// ================= SYSTICK API =================
// SysTick is initialised inside UART2_Init()
// but these are provided for external use if needed.
void SysTick_Init(void); 		 // Systick Initialisation
void     SysTick_Handler(void);  // ISR (increments millisecond counter)
uint32_t GetTick(void);          // Return current millisecond tick


#endif /* SYSTICK_CONFIG_H_ */
