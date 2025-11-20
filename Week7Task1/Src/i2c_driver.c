#include "i2c_driver.h"


/*
 * Pinout for the Nucleo-F411RE
 * PB8 ------ SCL
 * PB9 ------ SDA
 * I am using those for I2C1
 */

/*
 * From 18.6.8 in the reference manual
 * CCR = fclk/(2fscl)
 * CCR = 16E6/(2*100E3)
 * CCR = 80 or 0x50
 */

/*
 * From 18.6.9 in the reference manual
 * F = 16 MHz -> T = 62.5 ns
 * F = 100 kHz -> T
 * TRise = 1000 ns/ 62.5 ns +1
 * TRise = 17
 */

#define I2C_100kHz			0x50
#define Sm_MAX_RISE_TIME	17

void I2C1_init(void){


	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;


	GPIOB->MODER &= (~GPIO_MODER_MODER8_Msk) | ~(GPIO_MODER_MODER9_Msk);
	GPIOB->MODER |= GPIO_MODER_MODER8_1 | GPIO_MODER_MODER9_1;


	GPIOB->AFR[1] |= (4U<<GPIO_AFRH_AFSEL8_Pos);
	GPIOB->AFR[1] |= (4U<<GPIO_AFRH_AFSEL9_Pos);



	GPIOB->OTYPER |= (GPIO_OTYPER_OT8 | GPIO_OTYPER_OT9);
	GPIOB->PUPDR |= (GPIO_PUPDR_PUPD8_0 | GPIO_PUPDR_PUPD9_0);

	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

	I2C1->CR1 |= I2C_CR1_SWRST;

	I2C1->CR1 &= ~I2C_CR1_SWRST_Msk;

	I2C1->CR2 |= (1U << 4);

	I2C1->CCR = I2C_100kHz;

	I2C1->TRISE = Sm_MAX_RISE_TIME;

	I2C1->CR1 |= I2C_CR1_PE;

}

void I2C1_readByte(char saddr, char maddr, char* data){

	volatile int tmp;

	while(I2C1->SR2 & I2C_SR2_BUSY){}

	I2C1->CR1 |= I2C_CR1_START;

	while(!(I2C1->SR1 & I2C_SR1_SB)){}


	I2C1->DR = (saddr << 1);

	while(!(I2C1->SR1 & I2C_SR1_ADDR)){}

	tmp = I2C1->SR2;


	I2C1->DR = maddr;


	while(!(I2C1->SR1 & I2C_SR1_TXE)){}

	I2C1->CR1 |= I2C_CR1_START;

	while(!(I2C1->SR1 & I2C_SR1_SB)){}

	I2C1->DR = saddr << 1 | 1;

	while(!(I2C1->SR1 & I2C_SR1_ADDR)){}

	I2C1->CR1 &= ~I2C_CR1_ACK_Msk;

	tmp = I2C1->SR2;

	I2C1->CR1 |= I2C_CR1_STOP;

	while(!(I2C1->SR1 & I2C_SR1_RXNE)){}

	*data++ = I2C1->DR;
	(void)tmp;

}

void I2C1_burstRead(char saddr, char maddr, int n, char* data){
	volatile int tmp;


	while(I2C1->SR2 & I2C_SR2_BUSY){}

	I2C1->CR1 |= I2C_CR1_START;

	while(!(I2C1->SR1 & I2C_SR1_SB)){}

	I2C1->DR = (saddr << 1);


	while(!(I2C1->SR1 & I2C_SR1_ADDR)){}

	tmp = I2C1->SR2;

	while(!(I2C1->SR1 & I2C_SR1_TXE)){}

	I2C1->DR = maddr;

	while(!(I2C1->SR1 & I2C_SR1_TXE)){}

	I2C1->CR1 |= I2C_CR1_START;

	while(!(I2C1->SR1 & I2C_SR1_SB)){}

	I2C1->DR = saddr << 1 | 1;


	while(!(I2C1->SR1 & I2C_SR1_ADDR)){}

	tmp = I2C1->SR2;

	I2C1->CR1 |= I2C_CR1_ACK;

	while(n > 0U){

		if(n == 1U){
			I2C1->CR1 &= ~I2C_CR1_ACK;

			I2C1->CR1 |= I2C_CR1_STOP;

			while(!(I2C1->SR1 & I2C_SR1_RXNE)){}

			*data++ = I2C1->DR;
			break;
		}
		else{
			while(!(I2C1->SR1 & I2C_SR1_RXNE)){}
			*data++ = I2C1->DR;
			n--;
		}
	}

	(void)tmp;
}

void I2C1_burstWrite(char saddr, char maddr, int n, char* data){

	volatile int tmp;

	while(I2C1->SR2 & I2C_SR2_BUSY){}

	I2C1->CR1 |= I2C_CR1_START;

	while(!(I2C1->SR1 & I2C_SR1_SB)){}

	I2C1->DR = (saddr << 1);

	while(!(I2C1->SR1 & I2C_SR1_ADDR)){}

	tmp = I2C1->SR2;

	while(!(I2C1->SR1 & I2C_SR1_TXE)){}

	I2C1->DR = maddr;



	for(int i = 0; i < n; i++){
		while(!(I2C1->SR1 & I2C_SR1_TXE)){}
		I2C1->DR = *data++;
	}

	while(!(I2C1->SR1 & I2C_SR1_BTF)){}

	I2C1->CR1 |= I2C_CR1_STOP;
	(void)tmp;


}
