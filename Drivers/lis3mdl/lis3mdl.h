/*
 * lis3mdl.h
 *
 *  Created on: Jul 26, 2025
 *      Author: arvyd
 */

#ifndef DRIVERS_LIS3MDL_LIS3MDL_H_
#define DRIVERS_LIS3MDL_LIS3MDL_H_

#include <lis3mdl_device.h>
#include <stdint.h>

/**
 * @brief Enumerates the possible status codes for the LIS3MDL device processing function.
 * These codes indicate the outcome or current state of the `lis3mdl_process` function.
 */

typedef enum {
	LIS3MDL_PROCESS_OK = 0x00,
	LIS3MDL_PROCESS_ALL_DEVICES_IDLING = 0x01,
	LIS3MDL_PROCESS_WAITING_FOR_SPI_CPLT = 0x02,
	LIS3MDL_PROCESS_ERROR
}LIS3MDL_Process_Status_t;

LIS3MDL_Process_Status_t lis3mdl_process(LIS3MDL_Device *devices, uint8_t num_of_devices, volatile uint8_t *spi_cplt_flag);
int get_first_non_idling_device_index(LIS3MDL_Device *devices, uint8_t num_of_devices);
LIS3MDL_Data_Retrieval_State_t lis3mdl_get_magnetic_data(LIS3MDL_Device *devices, uint8_t num_of_devices, uint8_t dev_index, LIS3MDL_Magnetic_Data_t *results);
HAL_StatusTypeDef lis3mdl_read_reg(LIS3MDL_Device *devices, uint8_t num_of_devices, uint8_t device_index, uint8_t reg, uint8_t size);
HAL_StatusTypeDef lis3mdl_write_reg(LIS3MDL_Device *devices, uint8_t num_of_devices, uint8_t device_index, uint8_t reg, uint8_t *data, uint8_t size);
uint8_t lis3mdl_clear_data(LIS3MDL_Device *device);

#endif /* DRIVERS_LIS3MDL_LIS3MDL_H_ */
