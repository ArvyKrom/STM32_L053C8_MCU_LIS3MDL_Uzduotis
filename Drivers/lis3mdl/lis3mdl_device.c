/*
 * lis3mdl_device.c
 *
 *  Created on: Aug 2, 2025
 *      Author: arvyd
 */

#include "lis3mdl_device.h"

uint8_t lis3mdl_initialize_device_struct(LIS3MDL_Device *device, SPI_HandleTypeDef *hspi, GPIO_TypeDef *cs_gpio_port_handle, uint16_t cs_pin){
	if (device == NULL || hspi == NULL || cs_gpio_port_handle == NULL)
		return 1;

	device->state = LIS3MDL_INITIALIZING_OFFSET_REGS;

	device->reg_addr = 0;
	device->data_size = 0;
	device->read_data_available = 0;

	device->hspi = hspi;
	device->cs_gpio_port_handle = cs_gpio_port_handle;
	device->cs_pin = cs_pin;

	return 0;
}

uint8_t lis3mdl_setup_config_registers(LIS3MDL_Device *device, LIS3MDL_Init_Params input_params){
	return lis3mdl_put_params_into_registers(input_params, device->config_regs.offsets, device->config_regs.ctrls, device->config_regs.ints);
}
