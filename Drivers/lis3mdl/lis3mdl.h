/*
 * lis3mdl.h
 *
 *  Created on: Jul 26, 2025
 *      Author: arvyd
 */

#ifndef DRIVERS_LIS3MDL_LIS3MDL_H_
#define DRIVERS_LIS3MDL_LIS3MDL_H_

#include <stdint.h>
#include "lis3mdl_interface.h"

#define WHO_AM_I_REG_ADDR 0x0F
#define CTRL_REG1_REG_ADDR 0x20
#define CTRL_REG2_REG_ADDR 0x21
#define CTRL_REG3_REG_ADDR 0x22
#define CTRL_REG4_REG_ADDR 0x23
#define CTRL_REG5_REG_ADDR 0x24

#define LIS3MDL_READ_BIT 0x80
#define LIS3MDL_MD_BIT 0x40

int lis3mdl_get_who_am_i(LIS3MDL_Comm_Interface_t comm_interface);
LIS3MDL_Status_t lis3mdl_write_byte(LIS3MDL_Comm_Interface_t comm_interface, uint8_t *reg_addr, uint8_t *data);
LIS3MDL_Status_t lis3mdl_read_byte(LIS3MDL_Comm_Interface_t comm_interface, uint8_t *reg_addr, uint8_t *data);
LIS3MDL_Status_t lis3mdl_read_consecutive_bytes(LIS3MDL_Comm_Interface_t comm_interface, uint8_t *first_reg_addr, uint8_t *data, uint8_t size);

#endif /* DRIVERS_LIS3MDL_LIS3MDL_H_ */
