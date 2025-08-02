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

HAL_StatusTypeDef lis3mdl_read_reg(LIS3MDL_Device *device, uint8_t reg, uint8_t size);
uint8_t lis3mdl_clear_data(LIS3MDL_Device *device);

#endif /* DRIVERS_LIS3MDL_LIS3MDL_H_ */
