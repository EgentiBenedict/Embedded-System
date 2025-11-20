#include "stm32f411xe.h"
#include "myClockConf.h"


void setClock(void){

	// Turn on HSI
	RCC->CR |= RCC_CR_HSION;

	// Wait for HSI to be ready
	while(!(RCC->CR & RCC_CR_HSIRDY)){}

	// Temporarily make HSI the system clock

	RCC->CFGR &= ~RCC_CFGR_SW;
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI);

	// Disable PLL to configure it
	RCC->CR &= ~RCC_CR_PLLON;
	// Wait until PLL is fully stopped
	while (RCC->CR & RCC_CR_PLLRDY);

	/* Configure PLL */

	// Reset PLLCFGR register
	RCC->PLLCFGR = 0;

	// HSI as PLL source

	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSI;

	/*
	 * HSI = 16 MHz
	 * For system clock = 48 MHz:
	 * 16*192/4
	 * PLLN = 192
	 * PLLM = 16
	 * PLLP = 4 (0b01)
	 */

	RCC->PLLCFGR |= (16U << RCC_PLLCFGR_PLLM_Pos);
	RCC->PLLCFGR |= (192U << RCC_PLLCFGR_PLLN_Pos);
	RCC->PLLCFGR |= (1U << RCC_PLLCFGR_PLLP_Pos);

	// Enable PLL
	RCC->CR |= RCC_CR_PLLON;
	// Wait until PLL is ready
	while (!(RCC->CR & RCC_CR_PLLRDY)){}

	// Configure Flash latency (1 wait state for 48 MHz, Table 5, ref manual)
	FLASH->ACR |= FLASH_ACR_LATENCY_1WS;

	// 7. Switch SYSCLK to PLL
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);


}
