#include "stm32f4xx.h"
#include <stdio.h>
#include <stdint.h>
#include "adxl345.h"

//Raw 16-bit readings from the sensor (two's complement)
int16_t x,y,z;
//Converted values in g (floating-point)
float xg, yg, zg;

// Buffer that the I^2C read routine fills with 6 bytes
// [X_L, X_H, Y_L, Y_H, Z_L, Z_H]
extern uint8_t rec_data[6];

int main(void){

	adxl_init();  //Set up I^2C + configuration ADXL345 registers:
	              //  - power up (MEASURE bit)
				  //  - data format
	              // - output data rate / bandwidth

	while(1){
		// Read 6 consecutive data bytes starting at DATAXO (0x32)
		// The driver places them in rec_data[0 to 5]

		adxl_read_values(DATA_START_ADD);

        // Pack little-endian bytes into signed 16-bit values
		x = ((rec_data[1] << 8) | rec_data[0]);
		y = ((rec_data[3] << 8) | rec_data[2]);
		z = ((rec_data[5] << 8) | rec_data[4]);

		// Convert raw counts to g
		// With ADXL345 set to +_4g, sensitivity = 7.8 mg/LSB to 0.0078 g/count
		xg = x*0.0078;
		yg = y*0.0078;
		zg = z*0.0078;

	}
}

// THis task is an I^2C readout loop for an ADXL345 3-axis accelerometer.
// The codes read 6-bytes from  the sensor, pack them into signed 16-bit X/Y/Z values,
// then convert to g using the sensor's sensitivity.
