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

typedef enum {
	LIS3MDL_PROCESS_OK = 0x00,
	LIS3MDL_PROCESS_ALL_DEVICES_IDLING = 0x01,
	LIS3MDL_PROCESS_WAITING_FOR_SPI_CPLT = 0x02,
	LIS3MDL_PROCESS_ERROR
}LIS3MDL_Process_Status_t;

LIS3MDL_Process_Status_t lis3mdl_process(LIS3MDL_Device *devices, uint8_t num_of_devices, volatile uint8_t *spi_cplt_flag);
int get_first_non_idling_device_index(LIS3MDL_Device *devices, uint8_t num_of_devices);
HAL_StatusTypeDef lis3mdl_read_reg(LIS3MDL_Device *devices, uint8_t num_of_devices, uint8_t device_index, uint8_t reg, uint8_t size);
uint8_t lis3mdl_clear_data(LIS3MDL_Device *device);

#endif /* DRIVERS_LIS3MDL_LIS3MDL_H_ */
