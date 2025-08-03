/*
 * lis3mdl_init_params.c
 *
 *  Created on: Aug 2, 2025
 *      Author: arvyd
 */

#include <stdio.h>
#include "lis3mdl_init_params.h"
#include "lis3mdl_registers.h"

/**
  * @brief Sets default initialization parameters for the LIS3MDL sensor.
  * This function populates a `LIS3MDL_Init_Params` structure with a predefined
  * set of default values. These defaults aim to provide a common starting
  * configuration, often including disabled features or common operating modes.
  *
  * @param init_params Pointer to the `LIS3MDL_Init_Params` structure to be initialized.
  *
  * @retval 0 if successful, 1 if `init_params` is NULL.
  */

uint8_t lis3mdl_set_default_params(LIS3MDL_Init_Params *init_params){
	if(init_params == NULL)
		return 1;

	init_params->offset_x = 0;
	init_params->offset_y = 0;
	init_params->offset_z = 0;

	init_params->temp_en = 0;
	init_params->xy_operation_mode = LIS3MDL_MEDIUM_PERFORMANCE;
	init_params->output_data_rate = LIS3MDL_ODR_10;
	init_params->fast_odr = 0;
	init_params->self_test = 0;

	init_params->full_scale = LIS3MDL_FULL_SCALE_16_GAUSS;

	init_params->low_power_mode = 0;
	init_params->spi_interface_mode = 0;
	init_params->conversion_mode = LIS3MDL_CONTINIOUS_CONVERSION;

	init_params->z_operation_mode = LIS3MDL_MEDIUM_PERFORMANCE;

	init_params->fast_read = 0;
	init_params->block_data_update = 1;

	init_params->x_interrupt_generation = 0;
	init_params->y_interrupt_generation = 0;
	init_params->z_interrupt_generation = 0;
	init_params->interrupt_active_configuration = 0;
	init_params->latch_interrupt = 0;
	init_params->int_pin = 0;

	init_params->positive_thresh_x_enable = 0;
	init_params->positive_thresh_y_enable = 0;
	init_params->positive_thresh_z_enable = 0;
	init_params->negative_thresh_x_enable = 0;
	init_params->negative_thresh_y_enable = 0;
	init_params->negative_thresh_z_enable = 0;

	init_params->interrupt_threshold = 0;
	return 0;
}

/**
  * @brief Translates LIS3MDL initialization parameters into raw register byte values.
  *
  * This function takes a `LIS3MDL_Init_Params` structure and converts its
  * human-readable configuration values into the specific bit-field layouts
  * required for the LIS3MDL's offset, control, and interrupt registers.
  * The resulting byte arrays can then be used for SPI communication to configure
  * the sensor.
  *
  * @param init_params The `LIS3MDL_Init_Params` structure containing the desired configuration.
  * @param offset_regs Pointer to a `uint8_t` array of size 6 to store offset register bytes (OUT_X_L_M to OUT_Z_H_M).
  * @param ctrl_regs Pointer to a `uint8_t` array of size 5 to store control register bytes (CTRL_REG1 to CTRL_REG5).
  * @param int_regs Pointer to a `uint8_t` array of size 4 to store interrupt register bytes (INT_CFG_REG to INT_THS_H_REG).
  *
  * @retval 0 if successful, 1 if any of the output register pointers are NULL.
  */

uint8_t lis3mdl_put_params_into_registers(LIS3MDL_Init_Params init_params, uint8_t *offset_regs, uint8_t *ctrl_regs, uint8_t *int_regs){
	if(offset_regs == NULL || ctrl_regs == NULL || int_regs == NULL)
		return 1;

	offset_regs[0] = (uint8_t)(init_params.offset_x & 0x00FF);
	offset_regs[1] = (uint8_t)((init_params.offset_x & 0xFF00)>>8);
	offset_regs[2] = (uint8_t)(init_params.offset_y & 0x00FF);
	offset_regs[3] = (uint8_t)((init_params.offset_y & 0xFF00)>>8);
	offset_regs[4] = (uint8_t)(init_params.offset_z & 0x00FF);
	offset_regs[5] = (uint8_t)((init_params.offset_z & 0xFF00)>>8);

	ctrl_regs[0] = 0;
	ctrl_regs[0] |= (init_params.temp_en << 7) & LIS3MDL_TEMP_EN;
	ctrl_regs[0] |= (init_params.xy_operation_mode << 5) & LIS3MDL_XY_OPERATING_MODE;
	ctrl_regs[0] |= (init_params.output_data_rate << 2) & LIS3MDL_ODR;
	ctrl_regs[0] |= (init_params.fast_odr << 1) & LIS3MDL_FAST_ODR;
	ctrl_regs[0] |= (init_params.self_test << 0) & LIS3MDL_SELF_TEST;

	ctrl_regs[1] = 0;
	ctrl_regs[1] |= (init_params.full_scale << 5) & LIS3MDL_FULL_SCALE;

	ctrl_regs[2] = 0;
	ctrl_regs[2] |= (init_params.low_power_mode << 5) & LIS3MDL_LP;
	ctrl_regs[2] |= (init_params.spi_interface_mode << 2) & LIS3MDL_SIM;
	ctrl_regs[2] |= (init_params.conversion_mode << 0) & LIS3MDL_MD;

	ctrl_regs[3] = 0;
	ctrl_regs[3] |= (init_params.z_operation_mode << 2) & LIS3MDL_Z_OPERATING_MODE;

	ctrl_regs[4] = 0;
	ctrl_regs[4] |= (init_params.fast_read << 7) & LIS3MDL_FAST_READ;
	ctrl_regs[4] |= (init_params.fast_read << 6) & LIS3MDL_BDU;

	int_regs[0] = 0;
	int_regs[0] |= (init_params.x_interrupt_generation << 7) & LIS3MDL_XIEN;
	int_regs[0] |= (init_params.y_interrupt_generation << 6) & LIS3MDL_YIEN;
	int_regs[0] |= (init_params.z_interrupt_generation << 5) & LIS3MDL_YIEN;
	int_regs[0] |= 0x08; // Reserved
	int_regs[0] |= (init_params.interrupt_active_configuration << 2) & LIS3MDL_IEA;
	int_regs[0] |= (init_params.latch_interrupt << 1) & LIS3MDL_LIR;
	int_regs[0] |= (init_params.int_pin << 0) & LIS3MDL_IEN;

	int_regs[1] = 0;
	int_regs[1] |= (init_params.positive_thresh_x_enable << 7) & LIS3MDL_PTH_X;
	int_regs[1] |= (init_params.positive_thresh_y_enable << 6) & LIS3MDL_PTH_Y;
	int_regs[1] |= (init_params.positive_thresh_z_enable << 5) & LIS3MDL_PTH_Z;
	int_regs[1] |= (init_params.negative_thresh_x_enable << 4) & LIS3MDL_NTH_X;
	int_regs[1] |= (init_params.negative_thresh_y_enable << 3) & LIS3MDL_NTH_Y;
	int_regs[1] |= (init_params.negative_thresh_z_enable << 2) & LIS3MDL_NTH_Z;

	int_regs[2] = (uint8_t)(init_params.interrupt_threshold & 0x00FF);
	int_regs[3] = (uint8_t)((init_params.interrupt_threshold & 0xFF00)>>8);

	return 0;
}


