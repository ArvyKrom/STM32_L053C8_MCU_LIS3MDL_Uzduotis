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

/*
 * Enum used as a HAL_StatusTypeDef substitute
 */

typedef enum {
	LIS3MDL_OK       = 0x00U,
	LIS3MDL_ERROR    = 0x01U,
	LIS3MDL_BUSY     = 0x02U,
	LIS3MDL_TIMEOUT  = 0x03U,
} LIS3MDL_Status_t;

/*
 * Pointer to HAL_SPI_Transmit(hspi, pData, Size, Timeout)
 */

typedef LIS3MDL_Status_t (*LIS3MDL_SPI_Transmit_DMA_Func)(void *hande_data, uint8_t *tx_data, uint16_t size);

/*
 * Pointer to HAL_SPI_Receive(hspi, pData, Size, Timeout)
 */

typedef LIS3MDL_Status_t (*LIS3MDL_SPI_Receive_DMA_Func)(void *hande_data, uint8_t *rx_data, uint16_t size);

/*
 * Pointer to HAL_GPIO_WritePin(GPIOx, GPIO_Pin, PinState)
 */

typedef void (*LIS3MDL_GPIO_Write_Func)(void *port_handle_data, uint16_t pin, int state);

/*
 * Struct that holds all the neccesary information about an LIS3MDL device
 * Must be initialized and populated before use
 */

typedef struct {
	LIS3MDL_Init_Params init_params;
	LIS3MDL_State_t state;

	void *cs_gpio_port_handle;
	uint16_t cs_pin;

	void *spi_handle;
	LIS3MDL_SPI_Transmit_DMA_Func spi_transmit_dma;
	LIS3MDL_SPI_Receive_DMA_Func spi_receive_dma;
	LIS3MDL_GPIO_Write_Func gpio_write;

} LIS3MDL_Device;

#endif /* LIS3MDL_LIS3MDL_DEVICE_H_ */
