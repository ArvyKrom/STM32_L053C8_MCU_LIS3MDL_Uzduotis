/*
 * lis3mdl_state_machine.c
 *
 *  Created on: Jul 31, 2025
 *      Author: arvyd
 */

#include <lis3mdl_process_state_machine.h>

/**
  * @brief Manages the state transitions of the LIS3MDL device process based on SPI completion.
  *
  * This forms part of a state machine to manage complex multi-step SPI transactions
  * like initialization sequences or read/write operations.
  *
  * @param state Pointer to the current `LIS3MDL_Process_State_t` of the device.
  * This value will be updated to the next state upon successful transition.
  *
  * @retval LIS3MDL_STATE_CHANGE_OK if the state transition is valid and successful.
  * @retval LIS3MDL_STATE_CHANGE_INVALID_CHANGE if the current state does not allow
  * a valid transition upon SPI completion (e.g., unexpected state).
  */

LIS3MDL_State_Change_Error_t lis3mdl_change_state_due_to_spi_cplt(LIS3MDL_Process_State_t *state){
	switch(*state){
	case LIS3MDL_RESETTING_REGISTERS:
		*state = LIS3MDL_INITIALIZING_OFFSET_REGS;
		break;
	case LIS3MDL_INITIALIZING_OFFSET_REGS:
//		*state = LIS3MDL_INITIALIZING_CTRL_REGS;
		*state = LIS3MDL_INITIALIZING_CTRL_REGS;
		break;

	case LIS3MDL_INITIALIZING_CTRL_REGS:
		*state = LIS3MDL_INITIALIZING_INT_REGS;
		break;

	case LIS3MDL_INITIALIZING_INT_REGS:
		*state = LIS3MDL_IDLE;
		break;

	case LIS3MDL_SENDING_ADDRESS_TO_READ_FROM:
		*state = LIS3MDL_READING_DATA;
		break;

	case LIS3MDL_SENDING_ADDRESS_TO_WRITE_TO:
		*state = LIS3MDL_WRITING_DATA;
		break;

	case LIS3MDL_WRITING_DATA:
		*state = LIS3MDL_IDLE;
		break;

	case LIS3MDL_READING_DATA:
		*state = LIS3MDL_IDLE;
		break;

	default:
		return LIS3MDL_STATE_CHANGE_INVALID_CHANGE;
	}

	return LIS3MDL_STATE_CHANGE_OK;
}
