/*
 * lis3mdl_interface.h
 *
 *  Created on: Jul 27, 2025
 *      Author: arvyd
 *
 */

#ifndef LIS3MDL_LIS3MDL_DEVICE_H_
#define LIS3MDL_LIS3MDL_DEVICE_H_

#include <lis3mdl_process_state_machine.h>
#include <stdint.h>
#include "lis3mdl_init_params.h"
#include "main.h"

#define LIS3MDL_BUFFER_SIZE 6 // Since there are no more than 6 writable registers

/**
 * @brief Enumerates the states for LIS3MDL magnetic data retrieval process.
 */

typedef enum {
	LIS3MDL_DATA_AVAILABLE = 0x00,
	LIS3MDL_STARTING_STATUS_CHECK = 0x01,
	LIS3MDL_STATUS_CHECK_IN_PROGRESS = 0x02,
	LIS3MDL_STARTING_DATA_RETRIEVAL = 0x03,
	LIS3MDL_DATA_RETRIEVAL_IN_PROGRESS = 0x04,
	LIS3MDL_DATA_RETRIEVAL_ERROR = 0x05
}LIS3MDL_Data_Retrieval_State_t;

/**
 * @brief Structure representing a single LIS3MDL device and its current state.
 * This holds all necessary information for managing communication and data with the sensor.
 */

typedef struct {
	LIS3MDL_Config_regs config_regs;
	LIS3MDL_Process_State_t process_state;
	LIS3MDL_Data_Retrieval_State_t data_retrieval_state;

	uint8_t reg_addr;
	uint8_t tx[LIS3MDL_BUFFER_SIZE];
	uint8_t rx[LIS3MDL_BUFFER_SIZE];
	uint8_t data_size;

	GPIO_TypeDef *cs_gpio_port_handle;
	uint16_t cs_pin;
	SPI_HandleTypeDef *hspi;

} LIS3MDL_Device;

/**
 * @brief Structure to hold the 3-axis magnetic field data (X, Y, Z).
 * Data is typically represented as 16-bit signed integers.
 */

typedef struct{
	int16_t x;
	int16_t y;
	int16_t z;
}LIS3MDL_Magnetic_Data_t;

uint8_t lis3mdl_initialize_device_struct(LIS3MDL_Device *device, SPI_HandleTypeDef *hspi, GPIO_TypeDef *cs_gpio_port_handle, uint16_t cs_pin);
uint8_t lis3mdl_setup_config_registers(LIS3MDL_Device *device, LIS3MDL_Init_Params input_params);

#endif /* LIS3MDL_LIS3MDL_DEVICE_H_ */
