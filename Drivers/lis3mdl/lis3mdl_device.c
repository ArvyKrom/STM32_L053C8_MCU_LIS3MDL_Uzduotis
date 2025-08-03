/*
 * lis3mdl_device.c
 *
 *  Created on: Aug 2, 2025
 *      Author: arvyd
 */

#include "lis3mdl_device.h"
#include "string.h"

/**
  * @brief Initializes the LIS3MDL_Device structure with default values and hardware handles.
  *
  * @param device Pointer to the LIS3MDL_Device structure to initialize.
  * @param hspi Pointer to the SPI_HandleTypeDef structure associated with the LIS3MDL.
  * @param cs_gpio_port_handle Pointer to the GPIO_TypeDef for the Chip Select (CS) pin's port.
  * @param cs_pin GPIO pin number for the Chip Select (CS) pin.
  *
  * @retval 0 on success, 1 on error (e.g., NULL pointer).
  */

uint8_t lis3mdl_initialize_device_struct(LIS3MDL_Device *device, SPI_HandleTypeDef *hspi, GPIO_TypeDef *cs_gpio_port_handle, uint16_t cs_pin){
	if (device == NULL || hspi == NULL || cs_gpio_port_handle == NULL)
		return 1;

	device->process_state = LIS3MDL_RESETTING_REGISTERS;
	device->data_retrieval_state = LIS3MDL_STARTING_STATUS_CHECK;

	device->reg_addr = 0;
	device->data_size = 0;
	memset(device->rx, 0, LIS3MDL_BUFFER_SIZE);
	memset(device->tx, 0, LIS3MDL_BUFFER_SIZE);
	device->hspi = hspi;
	device->cs_gpio_port_handle = cs_gpio_port_handle;
	device->cs_pin = cs_pin;

	return 0;
}

/**
  * @brief Populates the LIS3MDL configuration registers within the device struct based on input parameters.
  * This function typically calls an internal helper function to map initialization parameters
  * to the specific register values for offsets, control registers, and interrupt registers.
  *
  * @param device Pointer to the LIS3MDL_Device structure whose config_regs member will be updated.
  * @param input_params A structure containing desired initialization parameters (e.g., ODR, Full Scale, etc.).
  *
  * @retval The return value from lis3mdl_put_params_into_registers (0 on success, non-zero on error).
  */

uint8_t lis3mdl_setup_config_registers(LIS3MDL_Device *device, LIS3MDL_Init_Params input_params){
	return lis3mdl_put_params_into_registers(input_params, device->config_regs.offsets, device->config_regs.ctrls, device->config_regs.ints);
}
