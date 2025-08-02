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

LIS3MDL_Process_Status_t lis3mdl_process(LIS3MDL_Device *devices, uint8_t num_of_devices, uint8_t *spi_cplt_flag){
	if(devices == NULL)
		return LIS3MDL_PROCESS_ERROR;

	static int dev_index = 0;
	if(dev_index == 0){
		dev_index = get_first_non_idling_device_index(devices, num_of_devices);
	}

	if(dev_index < 0)
		return LIS3MDL_PROCESS_ALL_DEVICES_IDLING;

	static uint8_t spi_transaction_started = 0;

	if(spi_transaction_started){
		if(!*spi_cplt_flag)
			return LIS3MDL_PROCESS_WAITING_FOR_SPI_CPLT;
		*spi_cplt_flag = 0;

		if(lis3mdl_change_state_due_to_spi_cplt(&devices[dev_index].state) == LIS3MDL_STATE_CHANGE_INVALID_CHANGE)
			return LIS3MDL_PROCESS_ERROR;

		if(devices[dev_index].state != LIS3MDL_WRITING_DATA && devices[dev_index].state != LIS3MDL_READING_DATA)
			devices[dev_index].cs_gpio_port_handle->BSRR = devices[dev_index].cs_pin; // Pulling CS High

		spi_transaction_started = 0;
		dev_index = 0;
		return LIS3MDL_PROCESS_OK;
	}

	// If spi_transaction was not started
	devices[dev_index].cs_gpio_port_handle->BSRR = (devices[dev_index].cs_pin) << 16; // Pulling CS Low
	spi_transaction_started = 1;
	switch(devices[dev_index].state){
	case LIS3MDL_INITIALIZING_OFFSET_REGS:
		// Start the SPI DMA for setting offsets
		return LIS3MDL_PROCESS_OK;
	case LIS3MDL_INITIALIZING_CTRL_REGS:
		// Start the SPI DMA for setting CTRL regs
		return LIS3MDL_PROCESS_OK;
	case LIS3MDL_INITIALIZING_INT_REGS:
		// Start the SPI DMA for setting INT regs
		return LIS3MDL_PROCESS_OK;
	case LIS3MDL_SENDING_ADDRESS_TO_WRITE_TO:
		// Start the SPI DMA
		return LIS3MDL_PROCESS_OK;
	case LIS3MDL_SENDING_ADDRESS_TO_READ_FROM:
		if(HAL_SPI_Transmit_DMA(devices[dev_index].hspi,&devices[dev_index].reg_addr, 1) != HAL_OK)
			return LIS3MDL_PROCESS_ERROR;
		return LIS3MDL_PROCESS_OK;
	case LIS3MDL_WRITING_DATA:
		// Start the SPI DMA
		return LIS3MDL_PROCESS_OK;
	case LIS3MDL_READING_DATA:
		if(HAL_SPI_Receive_DMA(devices[dev_index].hspi,devices[dev_index].rx, devices[dev_index].data_size) != HAL_OK)
			return LIS3MDL_PROCESS_ERROR;
		return LIS3MDL_PROCESS_OK;
	default:
		return LIS3MDL_PROCESS_ERROR;
	}

	return LIS3MDL_PROCESS_ERROR;
}

int get_first_non_idling_device_index(LIS3MDL_Device *devices, uint8_t num_of_devices){
	if(devices == NULL)
		return -1;

	for(int i=0; i<num_of_devices; i++){
		if(devices[i].state != LIS3MDL_IDLE)
			return i;
	}

	return -1;
}

HAL_StatusTypeDef lis3mdl_read_reg(LIS3MDL_Device *device, uint8_t reg, uint8_t size){
	if(device == NULL)
		return HAL_ERROR;

	if((reg & LIS3MDL_READ_BIT) == LIS3MDL_READ_BIT || (reg & LIS3MDL_MD_BIT) == LIS3MDL_MD_BIT)
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

	memset(device->tx, 0, LIS3MDL_BUFFER_SIZE);
	memset(device->rx, 0, LIS3MDL_BUFFER_SIZE);
	device->reg_addr = 0;
	device->data_size = 0;
	device->read_data_available = 0;
	return 0;
}

uint8_t
