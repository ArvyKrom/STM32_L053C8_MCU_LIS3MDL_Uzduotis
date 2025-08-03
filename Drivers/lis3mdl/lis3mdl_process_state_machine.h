/*
 * lis3mdl_state_machine.h
 *
 *  Created on: Jul 31, 2025
 *      Author: arvyd
 */

#ifndef LIS3MDL_LIS3MDL_PROCESS_STATE_MACHINE_H_
#define LIS3MDL_LIS3MDL_PROCESS_STATE_MACHINE_H_

/**
 * @brief Enumerates the different processing states for the LIS3MDL device.
 * This forms the core of a state machine to manage complex multi-step operations
 * like initialization, reading, and writing to the sensor.
 */

typedef enum {
	LIS3MDL_RESETTING_REGISTERS = 0x00,
	LIS3MDL_INITIALIZING_OFFSET_REGS = 0x01,
	LIS3MDL_INITIALIZING_CTRL_REGS = 0x02,
	LIS3MDL_INITIALIZING_INT_REGS = 0x03,
	LIS3MDL_IDLE = 0x04,
	LIS3MDL_SENDING_ADDRESS_TO_READ_FROM = 0x05,
	LIS3MDL_SENDING_ADDRESS_TO_WRITE_TO = 0x06,
	LIS3MDL_READING_DATA = 0x07,
	LIS3MDL_WRITING_DATA = 0x08
} LIS3MDL_Process_State_t;

/**
 * @brief Enumerates the possible return codes for state change operations.
 */

typedef enum {
	LIS3MDL_STATE_CHANGE_OK = 0x00,
	LIS3MDL_STATE_CHANGE_INVALID_CHANGE = 0x01
} LIS3MDL_State_Change_Error_t;

LIS3MDL_State_Change_Error_t lis3mdl_change_state_due_to_spi_cplt(LIS3MDL_Process_State_t *state);

#endif /* LIS3MDL_LIS3MDL_PROCESS_STATE_MACHINE_H_ */
