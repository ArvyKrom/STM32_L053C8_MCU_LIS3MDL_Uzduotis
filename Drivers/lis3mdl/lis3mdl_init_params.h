/*
 * lis3mdl_init_params.h
 *
 *  Created on: Jul 31, 2025
 *      Author: arvyd
 */

#ifndef LIS3MDL_LIS3MDL_INIT_PARAMS_H_
#define LIS3MDL_LIS3MDL_INIT_PARAMS_H_

typedef enum {
	LOW_POWER = 0x00,
	MEDIUM_PERFORMANCE = 0x01,
	HIGH_PERFORAMCE = 0x02,
	ULTRA_PERFORMACE = 0x03
} LIS3MDL_Operation_Mode;

typedef enum {
	ODR_0_625 = 0x00,
	ODR_1_25 = 0x01,
	ODR_2_5 = 0x02,
	ODR_5 = 0x03,
	ODR_10 = 0x04,
	ODR_20 = 0x05,
	ODR_40 = 0x06,
	ODR_80 = 0x07
} LIS3MDL_Output_Data_Rate;

typedef enum {
	FULL_SCALE_4_GAUSS = 0x00,
	FULL_SCALE_8_GAUSS = 0x01,
	FULL_SCALE_12_GAUSS = 0x02,
	FULL_SCALE_16_GAUSS = 0x03,
} LIS3MDL_Full_Scale;

typedef enum {
	CONTINIOUS_CONVERSION = 0x00,
	SINGLE_CONVERSION = 0x01,
	POWER_DOWN = 0x02
} LIS3MDL_Conversion_mode;

typedef struct {
	//Offset registers
	uint16_t offset_x;
	uint16_t offset_y;
	uint16_t offset_z;

	//CTRL_REG1
	uint8_t temp_en;
	LIS3MDL_Operation_Mode xy_operation_mode;
	LIS3MDL_Output_Data_Rate output_data_rate;
	uint8_t fast_odr;
	uint8_t self_test;

	//CTRL_REG1
	LIS3MDL_Full_Scale full_scale;

	//CTRL_REG3
	uint8_t low_power_mode;
	uint8_t spi_interface_mode;
	LIS3MDL_Conversion_mode conversion_mode;

	//CTRL_REG4
	LIS3MDL_Operation_Mode z_operation_mode;

	//CTRL_REG5
	uint8_t fast_read;
	uint8_t block_data_update;

}LIS3MDL_Init_Params;

#endif /* LIS3MDL_LIS3MDL_INIT_PARAMS_H_ */
