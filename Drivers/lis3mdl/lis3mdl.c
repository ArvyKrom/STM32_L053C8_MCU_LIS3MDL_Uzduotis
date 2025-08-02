///*
// * lis3mdl.c
// *
// *  Created on: Jul 26, 2025
// *      Author: arvyd
// */
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "lis3mdl.h"
#include "lis3mdl_registers.h"

uint8_t lis3mdl_process(LIS3MDL_Device *device, uint8_t *spi_tx_cplt_flag, uint8_t *spi_rx_cplt_flag){
	if(device == NULL)
		return 1;

	if(device->cs_gpio_port_handle->ODR & device->cs_pin == 0){ // If CS was low allow the change of state
		if(lis3mdl_change_state_due_to_spi_cplt(&device->state, spi_tx_cplt_flag, spi_rx_cplt_flag) == LIS3MDL_STATE_CHANGE_INVALID_CHANGE)
			return 1;
		return 0;
	}

	switch(device->state){
	case LIS3MDL_IDLE:
		device->cs_gpio_port_handle->BSRR = (device->cs_pin);
		return 0;
	}

	if(device->state == LIS3MDL_IDLE){
		device->cs_gpio_port_handle->BSRR = (device->cs_pin);
		return 0;
	}

	if(device->state == LIS3MDL_SENDING_ADDRESS_TO_WRITE_TO ||
	   device->state == LIS3MDL_SENDING_ADDRESS_TO_READ_FROM){

		if(device->hspi->Instance->SR & SPI_SR_BSY)
			return HAL_BUSY;

		device->cs_gpio_port_handle->BSRR = (device->cs_pin) << 16; // Pulling the CS low
	}
}

HAL_StatusTypeDef lis3mdl_read_reg(LIS3MDL_Device *device, uint8_t reg, uint8_t size){
	if(device == NULL)
		return HAL_ERROR;

	if(reg & LIS3MDL_READ_BIT == LIS3MDL_READ_BIT || reg & LIS3MDL_MD_BIT == LIS3MDL_MD_BIT)
		return HAL_ERROR;

	if(size < 1 || size > LIS3MDL_BUFFER_SIZE)
		return HAL_ERROR;

	if(lis3mdl_clear_data(device)!=0)
		return HAL_ERROR;

	device->reg_addr = reg | LIS3MDL_READ_BIT;
	if(size > 1)
		reg |= LIS3MDL_MD_BIT;
	device->data_size = size;

	device->state = LIS3MDL_SENDING_ADDRESS_TO_READ_FROM;

	return HAL_OK;

}

uint8_t lis3mdl_clear_data(LIS3MDL_Device *device){
	if(device == NULL)
		return 1;

	memcpy(device->tx, 0, LIS3MDL_BUFFER_SIZE);
	memcpy(device->rx, 0, LIS3MDL_BUFFER_SIZE);
	device->reg_addr = 0;
	device->data_size = 0;
	device->read_data_available = 0;
	return 0;
}
