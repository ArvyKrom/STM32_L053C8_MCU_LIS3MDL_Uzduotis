/*
 * lis3mdl_init_params.h
 *
 *  Created on: Jul 31, 2025
 *      Author: arvyd
 */
#include <stdint.h>

#ifndef LIS3MDL_LIS3MDL_INIT_PARAMS_H_
#define LIS3MDL_LIS3MDL_INIT_PARAMS_H_

/**
 * @brief Defines the operational modes for the LIS3MDL magnetometer,
 * affecting power consumption and measurement accuracy.
 */

typedef enum {
	LIS3MDL_LOW_POWER = 0x00,
	LIS3MDL_MEDIUM_PERFORMANCE = 0x01,
	LIS3MDL_HIGH_PERFORAMCE = 0x02,
	LIS3MDL_ULTRA_PERFORMACE = 0x03
} LIS3MDL_Operation_Mode;

/**
 * @brief Defines the output data rates (ODR) for magnetic field measurements.
 * A higher ODR means more frequent measurements.
 */

typedef enum {
	LIS3MDL_ODR_0_625 = 0x00,
	LIS3MDL_ODR_1_25 = 0x01,
	LIS3MDL_ODR_2_5 = 0x02,
	LIS3MDL_ODR_5 = 0x03,
	LIS3MDL_ODR_10 = 0x04,
	LIS3MDL_ODR_20 = 0x05,
	LIS3MDL_ODR_40 = 0x06,
	LIS3MDL_ODR_80 = 0x07
} LIS3MDL_Output_Data_Rate;

/**
 * @brief Defines the full-scale magnetic field measurement range.
 * This determines the maximum magnetic field the sensor can measure and its sensitivity.
 */

typedef enum {
	LIS3MDL_FULL_SCALE_4_GAUSS = 0x00,
	LIS3MDL_FULL_SCALE_8_GAUSS = 0x01,
	LIS3MDL_FULL_SCALE_12_GAUSS = 0x02,
	LIS3MDL_FULL_SCALE_16_GAUSS = 0x03,
} LIS3MDL_Full_Scale;

/**
 * @brief Defines the conversion modes for the LIS3MDL magnetometer.
 * This controls when and how measurements are performed.
 */

typedef enum {
	LIS3MDL_CONTINIOUS_CONVERSION = 0x00,
	LIS3MDL_SINGLE_CONVERSION = 0x01,
	LIS3MDL_POWER_DOWN = 0x02
} LIS3MDL_Conversion_mode;

/**
 * @brief Structure for initializing LIS3MDL device parameters.
 *
 * This struct provides a high-level, human-readable representation of
 * all configurable parameters for the LIS3MDL magnetic sensor,
 * corresponding to its various control and interrupt registers.
 * These parameters are used to generate the actual register values.
 */

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

	//INT_CFG
	uint8_t x_interrupt_generation;
	uint8_t y_interrupt_generation;
	uint8_t z_interrupt_generation;
	uint8_t interrupt_active_configuration;
	uint8_t latch_interrupt;
	uint8_t int_pin;

	//INT_SRC

	uint8_t positive_thresh_x_enable;
	uint8_t positive_thresh_y_enable;
	uint8_t positive_thresh_z_enable;
	uint8_t negative_thresh_x_enable;
	uint8_t negative_thresh_y_enable;
	uint8_t negative_thresh_z_enable;

	//INT_THS

	uint16_t interrupt_threshold;

}LIS3MDL_Init_Params;

/**
 * @brief Structure to store the raw byte values for LIS3MDL configuration registers.
 *
 * This struct is used internally to hold the processed byte arrays that are
 * directly transmitted to the LIS3MDL sensor via SPI to configure its behavior.
 */

typedef struct {
	uint8_t offsets[6];
	uint8_t ctrls[5];
	uint8_t ints[4];
}LIS3MDL_Config_regs;

uint8_t lis3mdl_set_default_params(LIS3MDL_Init_Params *init_params);
uint8_t lis3mdl_put_params_into_registers(LIS3MDL_Init_Params init_params, uint8_t *offset_regs, uint8_t *ctrl_regs, uint8_t *int_regs);

#endif /* LIS3MDL_LIS3MDL_INIT_PARAMS_H_ */
