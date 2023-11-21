/*
Still WIP!! Not yet ready to use
Contains A LOT OF pseudocode, just to illustrate what to do
Please change source code ASAP and remove this warning after it is finished!

MPU6500 Driver Library

Written by : Raditya Eka Putra
Copyright 2023

Any usage of this library, both in binary and/or source code 
(including but not limited to : copy, use as part of another program, sharing, 
modifying, selling, lending) shall not be performed unless under explicit 
permission from the author.

This driver performs any neccessary setup for MPU6500 MEMS-based IMU. this library is suitable
for MPU6500 and MPU9250, which contains the same IMU

*/

//sensor self-test register
#define SELF_TEST_X_GYRO	0x00
#define SELF_TEST_Y_GYRO	0x01
#define SELF_TEST_Z_GYRO	0x02
#define SELF_TEST_X_ACCEL	0x0d
#define SELF_TEST_Y_ACCEL	0x0e
#define SELF_TEST_Z_ACCEL	0x0f

//sensor offset register
#define XG_OFFSET_H			0x13
#define XG_OFFSET_L			0x14
#define YG_OFFSET_H			0x15
#define YG_OFFSET_L			0x16
#define ZG_OFFSET_H			0x17
#define ZG_OFFSET_L			0x18
#define XA_OFFSET_H			0x77
#define XA_OFFSET_L			0x78
#define YA_OFFSET_H			0x7a
#define YA_OFFSET_L			0x7b
#define ZA_OFFSET_H			0x7d
#define ZA_OFFSET_L			0x7e

//sensor setting, management, and status registers
#define SAMPLERATE_DIV		0x19
#define SENSOR_CONFIG		0x1a
#define GYRO_CONFIG			0x1b
#define ACCEL_CONFIG		0x1c
#define ACCEL2_CONFIG		0x1d
#define LP_ACCEL_ODR		0x1e
#define WOM_THRESHOLD		0x1f
#define FIFO_ENABLE			0x23
#define INT_PIN_CONFIG		0x37
#define INT_ENABLE			0x38
#define INT_STATUS			0x3a
#define SIGNAL_PATH_RESET	0x68
#define ACCEL_INTEL_CTRL	0x69
#define USER_CTRL			0x6a
#define POWER_MANAGEMENT_1	0x6b
#define POWER_MANAGEMENT_2	0x6c
#define FIFO_COUNT_H		0x72
#define FIFO_COUNT_L		0x73
#define FIFO_R_W			0x74
#define WHO_AM_I			0x75

//I2C Slave Management registers
#define I2C_SLV0_ADDR		0x25
#define I2C_SLV0_REG		0x26
#define I2C_SLV0_CTRL		0x27
#define I2C_SLV1_ADDR		0x28
#define I2C_SLV1_REG		0x29
#define I2C_SLV1_CTRL		0x2a
#define I2C_SLV2_ADDR		0x2b
#define I2C_SLV2_REG		0x2c
#define I2C_SLV2_CTRL		0x2d
#define I2C_SLV3_ADDR		0x2e
#define I2C_SLV3_REG		0x2f
#define I2C_SLV3_CTRL		0x30
#define I2C_SLV4_ADDR		0x31
#define I2C_SLV4_REG		0x32
#define I2C_SLV4_DO			0x33
#define I2C_SLV4_CTRL		0x34
#define I2C_SLV4_DI			0x35
#define I2C_SLV0_DO			0x63
#define I2C_SLV1_DO			0x64
#define I2C_SLV2_DO			0x65
#define I2C_SLV3_DO			0x66

//I2C Master management registers
#define I2C_MASTER_CTRL		0x24
#define I2C_MASTER_STATUS	0x36
#define I2C_MSTR_DLY_CTRL	0x67

//IMU data output registers
#define ACCEL_XOUT_H		0x3b
#define ACCEL_XOUT_L		0x3c
#define ACCEL_YOUT_H		0x3d
#define ACCEL_YOUT_L		0x3e
#define ACCEL_ZOUT_H		0x3f
#define ACCEL_ZOUT_L		0x40
#define TEMP_OUT_H			0x41
#define TEMP_OUT_L			0x42
#define GYRO_XOUT_H			0x43
#define GYRO_XOUT_L			0x44
#define GYRO_YOUT_H			0x45
#define GYRO_YOUT_L			0x46
#define GYRO_ZOUT_H			0x47
#define GYRO_ZOUT_L			0x48

//external sensor data storage register
#define EXT_SENS_DATA_00	0x49
#define EXT_SENS_DATA_01	0x4a
#define EXT_SENS_DATA_02	0x4b
#define EXT_SENS_DATA_03	0x4c
#define EXT_SENS_DATA_04	0x4d
#define EXT_SENS_DATA_05	0x4e
#define EXT_SENS_DATA_06	0x4f
#define EXT_SENS_DATA_07	0x50
#define EXT_SENS_DATA_08	0x51
#define EXT_SENS_DATA_09	0x52
#define EXT_SENS_DATA_10	0x53
#define EXT_SENS_DATA_11	0x54
#define EXT_SENS_DATA_12	0x55
#define EXT_SENS_DATA_13	0x56
#define EXT_SENS_DATA_14	0x57
#define EXT_SENS_DATA_15	0x58
#define EXT_SENS_DATA_16	0x59
#define EXT_SENS_DATA_17	0x5a
#define EXT_SENS_DATA_18	0x5b
#define EXT_SENS_DATA_19	0x5c
#define EXT_SENS_DATA_20	0x5d
#define EXT_SENS_DATA_21	0x5e
#define EXT_SENS_DATA_22	0x5f
#define EXT_SENS_DATA_23	0x60

//status flags
#define MPU6500_SUCCESS		0
#define MPU6500_FAILED		1

//peripheral library headers
#include "spi.h"

namespace MPU6500

int sensor_initialize(spiHandler sensor) {
	if (spi_read(sensor,WHO_AM_I,1) != 0x70) {
		log("Not an MPU6500");
		return MPU6500_FAILED;
	}
	else {
		spi_write(sensor, POWER_MANAGEMENT_2, data);
		spi_write(sensor, POWER_MANAGEMENT_1, data);
		delay_ms(100);
		spi_write(sensor, SIGNAL_PATH_RESET, data);
		delay_ms(100);
		if (spi_read(sensor, WHO_AM_I, 1) != 0x70) {
			log("Not an MPU6500");
			return MPU6500_FAILED;
		}
		else {
			return MPU6500_SUCCESS;
		} 
	}
}

int sensor_initialSetup(spiHandler sensor, int data) {
	spi_write(sensor, POWER_MANAGEMENT_1, data);
	spi_write(sensor, POWER_MANAGEMENT_2, data);
	spi_write(sensor, SENSOR_CONFIG, data);
	spi_write(sensor, GYRO_CONFIG, data);
	spi_write(sensor, ACCEL2_CONFIG, data);
	spi_write(sensor, ACCEL_CONFIG, data);
	spi_write(sensor, USER_CTRL, data);
	spi_write(sensor, SAMPLERATE_DIV, data);
	spi_write(sensor, FIFO_ENABLE, data);
	spi_write(sensor, ACCEL_INTEL_CTRL, data);
}

void sensor_logData(spiHandler sensor, int *pointerToBufferArray) {
	for (int i = 0; i < 6; i++) {
		*(p+i) = spi_read(sensor, FIFO_R_W);
	}
}

int sensor_setAccelConfig(spiHandler sensor, int data) {
	spi_write(sensor,ACCEL_CONFIG,data);
	spi_write(sensor,ACCEL2_CONFIG,data);
}

int sensor_getAccelConfig(spiHandler sensor, int data) {
	return spi_read(sensor,ACCEL_CONFIG);
}

int sensor_getAccelFilterConfig(spiHandler sensor, int data) {
	return spi_read(sensor,ACCEL2_CONFIG);
}

int sensor_setGyroConfig(spiHandler sensor, int data) {
	spi_write(sensor,GYRO_CONFIG,data);
}

int sensor_getGyroConfig(spiHandler sensor, int data) {
	return spi_read(sensor,GYRO_CONFIG);
}

int sensor_setGeneralConfig(spiHandler sensor, int data) {
	spi_write(sensor,SENSOR_CONFIG,data);
}

int sensor_getGeneralConfig(spiHandler sensor, int data) {
	return spi_read(sensor,SENSOR_CONFIG);
}

int sensor_setSampleRate(spiHandler sensor, int data) {
	spi_write(sensor,SAMPLERATE_DIV,data);
}

int sensor_getSampleRate(spiHandler sensor, int data) {
	return spi_read(sensor,SAMPLERATE_DIV);
}
