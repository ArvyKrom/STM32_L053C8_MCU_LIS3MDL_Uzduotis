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

/**
  * @brief Manages the state-driven communication and processing for LIS3MDL devices via SPI DMA.
  *
  * This function acts as a cooperative state machine, intended to be called repeatedly
  * in a non-blocking manner within a main loop or task. It orchestrates multi-step
  * operations such as device initialization sequences (register reset, configuration)
  * and data transfer (reading/writing registers), ensuring proper timing and sequencing
  * through SPI DMA.
  *
  * @param devices Pointer to an array of LIS3MDL_Device structures, each representing an LIS3MDL sensor
  * and holding its current state and configuration.
  * @param num_of_devices The total number of LIS3MDL devices managed in the `devices` array.
  * @param spi_cplt_flag A volatile pointer to a flag (typically global) that is set by the
  * SPI DMA transfer complete Interrupt Service Routine (ISR). This flag
  * signals that the previously initiated SPI DMA transaction has finished.
  *
  * @retval LIS3MDL_PROCESS_ERROR If an unrecoverable error occurs, such as a NULL `devices` pointer,
  * a failed HAL SPI DMA call, or an invalid state transition.
  * @retval LIS3MDL_PROCESS_ALL_DEVICES_IDLING If all managed LIS3MDL devices are currently in an
  * idle state, meaning no processing is pending.
  * @retval LIS3MDL_PROCESS_WAITING_FOR_SPI_CPLT If an SPI DMA transaction was initiated and is still
  * in progress, requiring further calls to this function
  * once the `spi_cplt_flag` is set by the ISR.
  * @retval LIS3MDL_PROCESS_OK If a processing step was successfully initiated (e.g., a DMA transfer started)
  * or successfully completed, and the state machine can progress.
  */

LIS3MDL_Process_Status_t lis3mdl_process(LIS3MDL_Device *devices, uint8_t num_of_devices, volatile uint8_t *spi_cplt_flag){
	if(devices == NULL)
		return LIS3MDL_PROCESS_ERROR;

	static int dev_index = 0;
	if(dev_index == 0){
		dev_index = get_first_non_idling_device_index(devices, num_of_devices);
	}

	if(dev_index < 0){
		dev_index = 0;
		return LIS3MDL_PROCESS_ALL_DEVICES_IDLING;
	}

	static uint8_t spi_transaction_started = 0;

	if(spi_transaction_started){
		if(!*spi_cplt_flag)
			return LIS3MDL_PROCESS_WAITING_FOR_SPI_CPLT;
		*spi_cplt_flag = 0;

		if(lis3mdl_change_state_due_to_spi_cplt(&devices[dev_index].process_state) == LIS3MDL_STATE_CHANGE_INVALID_CHANGE)
			return LIS3MDL_PROCESS_ERROR;

		if(devices[dev_index].process_state != LIS3MDL_WRITING_DATA && devices[dev_index].process_state != LIS3MDL_READING_DATA){
			devices[dev_index].cs_gpio_port_handle->BSRR = devices[dev_index].cs_pin; // Pulling CS High
		}

		spi_transaction_started = 0;
		dev_index = 0;
		return LIS3MDL_PROCESS_OK;
	}

	// If spi_transaction was not started
	devices[dev_index].cs_gpio_port_handle->BSRR = (devices[dev_index].cs_pin) << 16; // Pulling CS Low
	spi_transaction_started = 1;
	uint8_t tx[7] = {0};
	switch(devices[dev_index].process_state){
	case LIS3MDL_RESETTING_REGISTERS:
		tx[0] = LIS3MDL_CTRL_REG2_ADDR;
		tx[1] = LIS3MDL_REBOOT;
		if(HAL_SPI_Transmit_DMA(devices[dev_index].hspi,tx, 2) != HAL_OK)
			return LIS3MDL_PROCESS_ERROR;
		return LIS3MDL_PROCESS_OK;
	case LIS3MDL_INITIALIZING_OFFSET_REGS:
		tx[0] = LIS3MDL_OFFSET_X_REG_L_M_ADDR | LIS3MDL_MD_BIT;
		memcpy(tx + 1, devices[dev_index].config_regs.offsets, 6);
		if(HAL_SPI_Transmit_DMA(devices[dev_index].hspi,tx, 7) != HAL_OK)
			return LIS3MDL_PROCESS_ERROR;
		HAL_Delay(8); // Magic wait time, don't delete and don't ask
		return LIS3MDL_PROCESS_OK;

	case LIS3MDL_INITIALIZING_CTRL_REGS:
		tx[0] = LIS3MDL_CTRL_REG1_ADDR | LIS3MDL_MD_BIT;
		memcpy(tx + 1, devices[dev_index].config_regs.ctrls, 5);
		if(HAL_SPI_Transmit_DMA(devices[dev_index].hspi,tx, 6) != HAL_OK)
			return LIS3MDL_PROCESS_ERROR;
		HAL_Delay(8); // Magic wait time, don't delete and don't ask
		return LIS3MDL_PROCESS_OK;

	case LIS3MDL_INITIALIZING_INT_REGS:
		tx[0] = LIS3MDL_INT_CFG_REG_ADDR| LIS3MDL_MD_BIT;
		memcpy(tx + 1, devices[dev_index].config_regs.ints, 4);
		if(HAL_SPI_Transmit_DMA(devices[dev_index].hspi,tx, 5) != HAL_OK)
			return LIS3MDL_PROCESS_ERROR;
		HAL_Delay(8); // Magic wait time, don't delete and don't ask
		return LIS3MDL_PROCESS_OK;

	case LIS3MDL_SENDING_ADDRESS_TO_WRITE_TO:
		if(HAL_SPI_Transmit_DMA(devices[dev_index].hspi,&devices[dev_index].reg_addr, 1) != HAL_OK)
					return LIS3MDL_PROCESS_ERROR;
		return LIS3MDL_PROCESS_OK;

	case LIS3MDL_SENDING_ADDRESS_TO_READ_FROM:
		if(HAL_SPI_Transmit_DMA(devices[dev_index].hspi,&devices[dev_index].reg_addr, 1) != HAL_OK)
			return LIS3MDL_PROCESS_ERROR;
		return LIS3MDL_PROCESS_OK;

	case LIS3MDL_WRITING_DATA:
		tx[0] = devices[dev_index].tx[0];
		if(HAL_SPI_Transmit_DMA(devices[dev_index].hspi,tx, 1) != HAL_OK)
					return LIS3MDL_PROCESS_ERROR;
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

/**
  * @brief Manages the state machine for asynchronously retrieving magnetic data from a LIS3MDL device.
  *
  * This function should be called repeatedly in a non-blocking loop. It progresses through
  * a sequence of states:
  * 1. Initiating a read of the status register to check for new data.
  * 2. Waiting for the status register read to complete and checking the data-ready flag.
  * 3. Initiating the read of the actual X, Y, Z magnetic data.
  * 4. Waiting for the magnetic data read to complete and parsing the results.
  *
  * @param devices Pointer to the array of LIS3MDL_Device structures.
  * @param num_of_devices The total number of devices in the `devices` array.
  * @param dev_index The index of the specific LIS3MDL device within the `devices` array
  * for which data is to be retrieved.
  * @param results Pointer to a `LIS3MDL_Magnetic_Data_t` structure where the
  * retrieved X, Y, Z magnetic field values will be stored upon successful completion.
  *
  * @retval LIS3MDL_DATA_RETRIEVAL_ERROR If `devices` or `results` are NULL, or if an
  * unexpected state is encountered.
  * @retval LIS3MDL_STARTING_STATUS_CHECK If the function successfully initiated a status
  * register read or needs to restart the status check because data wasn't ready.
  * @retval LIS3MDL_STATUS_CHECK_IN_PROGRESS If the status register read is ongoing (waiting
  * for the `lis3mdl_process` to complete the underlying SPI transaction).
  * @retval LIS3MDL_STARTING_DATA_RETRIEVAL If the function successfully initiated a magnetic
  * data read after determining data is available.
  * @retval LIS3MDL_DATA_RETRIEVAL_IN_PROGRESS If the magnetic data read is ongoing (waiting
  * for the `lis3mdl_process` to complete the underlying SPI transaction).
  * @retval LIS3MDL_DATA_AVAILABLE If magnetic data has been successfully retrieved and
  * parsed into the `results` structure. The process then resets to start a new status check.
  */

LIS3MDL_Data_Retrieval_State_t lis3mdl_get_magnetic_data(LIS3MDL_Device *devices, uint8_t num_of_devices, uint8_t dev_index, LIS3MDL_Magnetic_Data_t *results){
	if(devices == NULL || results == NULL){
		return LIS3MDL_DATA_RETRIEVAL_ERROR;
	}

	switch(devices[dev_index].data_retrieval_state){
	case LIS3MDL_STARTING_STATUS_CHECK:
		if(lis3mdl_read_reg(devices, num_of_devices, dev_index, LIS3MDL_STATUS_REG_ADDR, 1) == HAL_OK){
			devices[dev_index].data_retrieval_state = LIS3MDL_STATUS_CHECK_IN_PROGRESS;
			return LIS3MDL_STATUS_CHECK_IN_PROGRESS;
		}
		return LIS3MDL_STARTING_STATUS_CHECK;

	case LIS3MDL_STATUS_CHECK_IN_PROGRESS:
		if(get_first_non_idling_device_index(devices, num_of_devices) < 0){ // Every device is idling including this one
			if(devices[dev_index].rx[0] & LIS3MDL_ZYXDA){ // Data available bit from status register
				devices[dev_index].data_retrieval_state = LIS3MDL_STARTING_DATA_RETRIEVAL;
				return LIS3MDL_STARTING_DATA_RETRIEVAL;
			}
			devices[dev_index].data_retrieval_state = LIS3MDL_STARTING_STATUS_CHECK; // Data is not yet available reread the status reg until it is available
			return LIS3MDL_STARTING_STATUS_CHECK;
		}
		return LIS3MDL_STATUS_CHECK_IN_PROGRESS;

	case LIS3MDL_STARTING_DATA_RETRIEVAL:
		if(lis3mdl_read_reg(devices, num_of_devices, dev_index, LIS3MDL_OUT_X_L_ADDR, 6) == HAL_OK){
			devices[dev_index].data_retrieval_state = LIS3MDL_DATA_RETRIEVAL_IN_PROGRESS;
			return LIS3MDL_DATA_RETRIEVAL_IN_PROGRESS;
		}
		return LIS3MDL_STARTING_DATA_RETRIEVAL;

	case LIS3MDL_DATA_RETRIEVAL_IN_PROGRESS:
		if(get_first_non_idling_device_index(devices, num_of_devices) < 0){ // Every device is idling including this one
			results->x = (devices[dev_index].rx[0]) | (devices[dev_index].rx[1] << 8);
			results->y = (devices[dev_index].rx[2]) | (devices[dev_index].rx[3] << 8);
			results->z = (devices[dev_index].rx[4]) | (devices[dev_index].rx[5] << 8);

			devices[dev_index].data_retrieval_state = LIS3MDL_STARTING_STATUS_CHECK;
			return LIS3MDL_DATA_AVAILABLE;
		}
		return LIS3MDL_DATA_RETRIEVAL_IN_PROGRESS;
	default:
		return LIS3MDL_DATA_RETRIEVAL_ERROR;
	}

}

/**
  * @brief Finds the index of the first LIS3MDL device in the array that is not in an idle state.
  *
  * This function iterates through a given array of LIS3MDL_Device structures and
  * returns the index of the first device whose `process_state` is not `LIS3MDL_IDLE`.
  * It's typically used in a cooperative multitasking environment to determine which
  * device needs attention for its ongoing SPI transactions or initialization sequences.
  *
  * @param devices Pointer to an array of LIS3MDL_Device structures.
  * @param num_of_devices The total number of LIS3MDL devices in the `devices` array.
  *
  * @retval The zero-based index of the first non-idling device.
  * @retval -1 if all devices in the array are in the `LIS3MDL_IDLE` state,
  * or if the `devices` pointer is NULL.
  */

int get_first_non_idling_device_index(LIS3MDL_Device *devices, uint8_t num_of_devices){
	if(devices == NULL)
		return -1;

	for(int i=0; i<num_of_devices; i++){
		if(devices[i].process_state != LIS3MDL_IDLE)
			return i;
	}

	return -1;
}

/**
  * @brief Prepares a LIS3MDL device for a register read operation.
  *
  * This function sets up the necessary parameters within the LIS3MDL_Device structure
  * to prepare for reading one or more registers from the sensor. It does not
  * initiate the SPI communication directly; instead, it configures the device's
  * state and parameters so that a subsequent call to `lis3mdl_process()` can
  * execute the actual SPI read transaction via DMA.
  *
  * @param devices Pointer to an array of LIS3MDL_Device structures.
  * @param num_of_devices The total number of LIS3MDL devices in the `devices` array.
  * @param device_index The index of the specific LIS3MDL device within the `devices` array
  * for which the read operation is being prepared.
  * @param reg The starting address of the register(s) to be read from the LIS3MDL sensor.
  * This should be the raw register address without the read/multi-byte bits.
  * @param size The number of bytes (registers) to read starting from the `reg` address.
  *
  * @retval HAL_OK If the device is successfully prepared for the read operation.
  * @retval HAL_ERROR If any input parameter is invalid (e.g., NULL `devices` pointer,
  * invalid `reg` flags, or `size` out of bounds), or if `lis3mdl_clear_data` fails.
  * @retval HAL_BUSY If any LIS3MDL device (including the target `device_index`) is
  * currently busy with another ongoing SPI transaction (i.e., not in `LIS3MDL_IDLE` state).
  */

HAL_StatusTypeDef lis3mdl_read_reg(LIS3MDL_Device *devices, uint8_t num_of_devices, uint8_t device_index, uint8_t reg, uint8_t size){
	if(devices == NULL)
		return HAL_ERROR;

	if(get_first_non_idling_device_index(devices, num_of_devices) >= 0)
		return HAL_BUSY;

	if((reg & LIS3MDL_READ_BIT) == LIS3MDL_READ_BIT || (reg & LIS3MDL_MD_BIT) == LIS3MDL_MD_BIT)
		return HAL_ERROR;

	if(size < 1 || size > LIS3MDL_BUFFER_SIZE)
		return HAL_ERROR;

	if(lis3mdl_clear_data(&devices[device_index])!=0)
		return HAL_ERROR;

	devices[device_index].reg_addr = reg | LIS3MDL_READ_BIT;
	if(size > 1)
		devices[device_index].reg_addr |= LIS3MDL_MD_BIT;
	devices[device_index].data_size = size;

	devices[device_index].process_state = LIS3MDL_SENDING_ADDRESS_TO_READ_FROM;

	return HAL_OK;

}

/**
  * @brief Clears the data buffers and resets transfer-related parameters within a LIS3MDL_Device structure.
  *
  * This function zeroes out the transmit (tx) and receive (rx) buffers,
  * and resets the register address, data size, and data available flag
  * for a specific LIS3MDL device. It's typically called before initiating
  * a new SPI transaction to ensure no residual data or flags interfere.
  *
  * @param device Pointer to the LIS3MDL_Device structure whose data needs to be cleared.
  *
  * @retval 0 if the data was successfully cleared.
  * @retval 1 if the `device` pointer is NULL.
  */

uint8_t lis3mdl_clear_data(LIS3MDL_Device *device){
	if(device == NULL)
		return 1;

	memset(device->tx, 0, LIS3MDL_BUFFER_SIZE);
	memset(device->rx, 0, LIS3MDL_BUFFER_SIZE);
	device->reg_addr = 0;
	device->data_size = 0;
	return 0;
}
