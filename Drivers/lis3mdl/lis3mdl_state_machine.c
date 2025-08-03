/*
 * lis3mdl_state_machine.c
 *
 *  Created on: Jul 31, 2025
 *      Author: arvyd
 */

#include "lis3mdl_state_machine.h"

LIS3MDL_State_Change_Error_t lis3mdl_change_state_due_to_spi_cplt(LIS3MDL_State_t *state){
	switch(*state){
	case LIS3MDL_RESETTING_REGISTERS:
		*state = LIS3MDL_INITIALIZING_OFFSET_REGS;
		break;
	case LIS3MDL_INITIALIZING_OFFSET_REGS:
//		*state = LIS3MDL_INITIALIZING_CTRL_REGS;
		*state = LIS3MDL_IDLE;
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
