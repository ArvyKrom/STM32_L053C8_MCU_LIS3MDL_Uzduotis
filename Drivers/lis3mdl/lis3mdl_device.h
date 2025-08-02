/*
 * lis3mdl_interface.h
 *
 *  Created on: Jul 27, 2025
 *      Author: arvyd
 *
 *      Used to enable driver's portability across different STM families.
 *      When a user exploits the driver in his code he should declare and populate
 *      a LIS3MDL_Comm_Interface_t struct and then later on use to communicate with
 *      the driver.
 */

#ifndef LIS3MDL_LIS3MDL_DEVICE_H_
#define LIS3MDL_LIS3MDL_DEVICE_H_

#include <stdint.h>
#include "lis3mdl_state_machine.h"
#include "lis3mdl_init_params.h"
#include "main.h"

#define LIS3MDL_BUFFER_SIZE 6 // User shouldn't ever need to write to or read from more than 6 registers consecutively

/*
 * Struct that holds all the neccesary information about an LIS3MDL device
 * Must be initialized and populated before use
 */

typedef struct {
	LIS3MDL_Init_Params init_params;
	LIS3MDL_State_t state;

	uint8_t reg_addr;
	uint8_t tx[LIS3MDL_BUFFER_SIZE];
	uint8_t rx[LIS3MDL_BUFFER_SIZE];
	uint8_t data_size;
	uint8_t read_data_available;

	GPIO_TypeDef *cs_gpio_port_handle;
	uint16_t cs_pin;
	SPI_HandleTypeDef *hspi;

} LIS3MDL_Device;

#endif /* LIS3MDL_LIS3MDL_DEVICE_H_ */
