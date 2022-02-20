/*
 * mma8451.c - Define functions to get values from accelerometer
 *
 *  Created on: Apr 28, 2021
 *      Author: saloni shah
 */

#include <MKL25Z4.H>
#include "mma8451.h"
#include "i2c.h"
#include "delay.h"
#include "fsl_debug_console.h"
#include <math.h>

//defined variables for device and register address
#define MMA_ADDR 0x3A

#define REG_XHI 0x01
#define REG_XLO 0x02
#define REG_YHI 0x03
#define REG_YLO 0x04
#define REG_ZHI	0x05
#define REG_ZLO 0x06

#define REG_WHOAMI 0x0D
#define REG_CTRL1  0x2A
#define REG_CTRL4  0x2D

#define WHOAMI 0x1A

#define COUNTS_PER_G (4096.0)
#define M_PI (3.14159265)

extern signed int acc_X, acc_Y, acc_Z;

//mma data ready
extern uint32_t DATA_READY;

//initializes mma8451 sensor
//i2c has to already be enabled
int init_mma() {

	//set active mode, 14 bit samples and 800 Hz ODR
	i2c_write_byte(MMA_ADDR, REG_CTRL1, 0x01);
	return 1;
}

void read_full_xyz_1() {
	int i;
	uint8_t data[6];
	int16_t temp[3];

	i2c_start();
	i2c_read_setup(MMA_ADDR , REG_XHI);

	// Read five bytes in repeated mode
	for( i=0; i<5; i++)	{
		data[i] = i2c_repeated_read(0);
	}

	// Read last byte ending repeated mode
	data[i] = i2c_repeated_read(1);

	for ( i=0; i<3; i++ ) {
		temp[i] = (int16_t) ((data[2*i]<<8) | data[2*i+1]);
	}

	// Align for 14 bits
	acc_X = temp[0]/4;
	acc_Y = temp[1]/4;
	acc_Z = temp[2]/4;
}

void read_full_xyz_2() {
	int i;
	uint8_t data[6];
	int16_t temp[3];

	i2c_start();
	i2c_read_setup(MMA_ADDR , REG_YHI);

	// Read five bytes in repeated mode
	for( i=0; i<5; i++)	{
		data[i] = i2c_repeated_read(0);
	}
	// Read last byte ending repeated mode
	data[i] = i2c_repeated_read(1);

	for ( i=0; i<3; i++ ) {
		temp[i] = (int16_t) ((data[2*i]<<8) | data[2*i+1]);
	}

	// Align for 14 bits
	acc_X = temp[0]/4;
	acc_Y = temp[1]/4;
	acc_Z = temp[2]/4;
}

float convert_to_roll() {
	float	ay = acc_Y/COUNTS_PER_G,
			az = acc_Z/COUNTS_PER_G;

	return atan2(ay, az)*180/M_PI;
}

float convert_to_pitch() {
	float ax = acc_X/COUNTS_PER_G,
			ay = acc_Y/COUNTS_PER_G,
			az = acc_Z/COUNTS_PER_G;

	return atan2(ax, sqrt(ay*ay + az*az))*180/M_PI;
}
