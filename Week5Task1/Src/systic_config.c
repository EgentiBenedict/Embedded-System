#include "systick_config.h"

#define SYSTICK_LOAD_VALUE 16000-1

void delayMS(int timeMS){
	SysTick->LOAD |=  SYSTICK_LOAD_VALUE;

	SysTick->VAL = 0;

	SysTick->CTRL |= (1 << SysTick_CTRL_CLKSOURCE_Pos);

	SysTick->CTRL |= (1 << SysTick_CTRL_ENABLE_Pos);

	for(int i = 0; i < timeMS; i++){
		while(!(SysTick->CTRL & (1<<SysTick_CTRL_COUNTFLAG_Pos)));
	}

	SysTick->CTRL = 0;
}
