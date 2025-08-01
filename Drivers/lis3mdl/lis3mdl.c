///*
// * lis3mdl.c
// *
// *  Created on: Jul 26, 2025
// *      Author: arvyd
// */
#include <stdint.h>
#include <stdio.h>
#include "lis3mdl.h"

LIS3MDL_Status_t read_who_am_i_reg(LIS3MDL_Device lis3mdl, uint8_t *who_am_i_reg_val){
	lis3mdl.gpio_write(lis3mdl.cs_gpio_port_handle, lis3mdl.cs_pin,0);
}
