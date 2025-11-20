#include "adxl345.h"

char data;

uint8_t rec_data[6];

void adxl_read_address(uint8_t reg){

	I2C1_readByte(DEVICE_ADDR, reg, &data);
}

void adxl_write(uint8_t reg, char value){

	char data[1];
	data[0] = value;
	I2C1_burstWrite(DEVICE_ADDR, reg, 1, data);

}

void adxl_read_values(uint8_t reg){

	I2C1_burstRead(DEVICE_ADDR, reg, 6, (char*)rec_data);

}

void adxl_init(void){

	I2C1_init();

	adxl_read_address(DEVID);

	adxl_write(DATA_FORMAT, FOUR_G);

	adxl_write(PWR_CTL, RESET);
	adxl_write(PWR_CTL, SET_MEASURE_B);

}
