///*
// * lis3mdl.c
// *
// *  Created on: Jul 26, 2025
// *      Author: arvyd
// */
#include <stdint.h>
#include <stdio.h>
#include "lis3mdl.h"

/**
 * @brief Retrieves LIS3MDL chip's WHO_AM_I_REG value
 *
 * @param[in] comm_interface The LIS3MDL_Comm_Interface_t struct
 *
 * @retval WHO_AM_I_REG default value (0x3D) if successful
 * @retval -ERROR_code if something goes wrong
 */

int lis3mdl_get_who_am_i(LIS3MDL_Comm_Interface_t comm_interface){
	uint8_t tx = WHO_AM_I_REG_ADDR;
	uint8_t rx;
	LIS3MDL_Status_t ret;

	ret = lis3mdl_read_byte(comm_interface, &tx, &rx);
	if (ret != LIS3MDL_OK)
		return -ret;

	return (int)rx;
}



/**
 * @brief Writes one byte of data into an LIS3MDL register
 *
 * @param[in] comm_interface The LIS3MDL_Comm_Interface_t struct
 * @param[in] reg_addr Pointer to a uint8_t type variable holding a register address
 * @param[in] data Pointer to a uint8_t type variable holding data to be written to register
 *
 * @retval LIS3MDL_OK If successful
 * @retval LIS3MDL error code If something goes wrong
 *
 * @note reg_addr must have two MSBs empty (0)
 */

LIS3MDL_Status_t lis3mdl_write_byte(LIS3MDL_Comm_Interface_t comm_interface, uint8_t *reg_addr, uint8_t *data){
	LIS3MDL_Status_t ret;

	// Make sure user sends reg_addr that consists only of 6 Lower bits
	if((*reg_addr & LIS3MDL_READ_BIT) == LIS3MDL_READ_BIT || (*reg_addr & LIS3MDL_MD_BIT) == LIS3MDL_MD_BIT)
		return LIS3MDL_ERROR;

	// Pulling CS low
	comm_interface.gpio_write(comm_interface.cs_gpio_port_handle, comm_interface.cs_pin, 0);

	// Writing reg_addr with READ and MD bits off
	ret = comm_interface.spi_transmit(comm_interface.spi_handle, reg_addr, 1, comm_interface.spi_timeout_ms);
	if (ret != 0)
		return ret;

	// Writing data byte
	ret = comm_interface.spi_transmit(comm_interface.spi_handle, data, 1, comm_interface.spi_timeout_ms);
	if (ret != 0)
		return ret;

	// Pulling CS high
	comm_interface.gpio_write(comm_interface.cs_gpio_port_handle, comm_interface.cs_pin, 1);

	return ret;
}

/**
 * @brief Reads one byte of data from an LIS3MDL register
 *
 * @param[in] comm_interface The LIS3MDL_Comm_Interface_t struct
 * @param[in] reg_addr Pointer to a uint8_t type variable holding a register address
 * @param[out] data Pointer to a uint8_t type variable that will hold data read from the register
 *
 * @retval LIS3MDL_OK If successful
 * @retval LIS3MDL error code If something goes wrong
 *
 * @note reg_addr must have two MSBs empty (0)
 */


LIS3MDL_Status_t lis3mdl_read_byte(LIS3MDL_Comm_Interface_t comm_interface, uint8_t *reg_addr, uint8_t *data){
	LIS3MDL_Status_t ret;
	uint8_t tx;

	// Make sure user sends reg_addr that consists only of 6 Lower bits
	if((*reg_addr & LIS3MDL_READ_BIT) == LIS3MDL_READ_BIT || (*reg_addr & LIS3MDL_MD_BIT) == LIS3MDL_MD_BIT)
		return LIS3MDL_ERROR;

	tx = *reg_addr | LIS3MDL_READ_BIT;

	// Pulling CS low
	comm_interface.gpio_write(comm_interface.cs_gpio_port_handle, comm_interface.cs_pin, 0);

	// Writing reg_addr with READ bit on and MD off
	ret = comm_interface.spi_transmit(comm_interface.spi_handle, &tx, 1, comm_interface.spi_timeout_ms);
	if (ret != 0)
		return ret;

	// Reading byte
	ret = comm_interface.spi_receive(comm_interface.spi_handle, data, 1, comm_interface.spi_timeout_ms);
	if (ret != 0)
		return ret;

	// Pulling CS high
	comm_interface.gpio_write(comm_interface.cs_gpio_port_handle, comm_interface.cs_pin, 1);

	return ret;
}

/**
 * @brief Reads size amount of bytes from size amount of consecutive LIS3MDL registers
 *
 * @param[in] comm_interface The LIS3MDL_Comm_Interface_t struct
 * @param[in] first_reg_addr Pointer to a uint8_t type variable holding first register's address
 * @param[out] data Pointer to a uint8_t type array that will hold data read from the registers
 * @param[in] size Number of registers that need to read
 *
 * @retval LIS3MDL_OK If successful
 * @retval LIS3MDL error code If something goes wrong
 *
 * @note first_reg_addr must have two MSBs empty (0)
 */

LIS3MDL_Status_t lis3mdl_read_consecutive_bytes(LIS3MDL_Comm_Interface_t comm_interface, uint8_t *first_reg_addr, uint8_t *data, uint8_t size){
	LIS3MDL_Status_t ret;
	uint8_t tx;

	// Make sure user sends reg_addr that consists only of 6 Lower bits
	if((*first_reg_addr & LIS3MDL_READ_BIT) == LIS3MDL_READ_BIT || (*first_reg_addr & LIS3MDL_MD_BIT) == LIS3MDL_MD_BIT)
		return LIS3MDL_ERROR;

	tx = *first_reg_addr | LIS3MDL_READ_BIT | LIS3MDL_MD_BIT;

	// Pulling CS low
	comm_interface.gpio_write(comm_interface.cs_gpio_port_handle, comm_interface.cs_pin, 0);

	// Writing reg_addr with READ and MD bits on
	ret = comm_interface.spi_transmit(comm_interface.spi_handle, &tx, 1, comm_interface.spi_timeout_ms);
	if (ret != 0)
		return ret;

	// Reading size of consecutive bytes
	for(int i=0; i<size; i++){
		ret = comm_interface.spi_receive(comm_interface.spi_handle, data+i, 1, comm_interface.spi_timeout_ms);
		if (ret != 0)
			return ret;
	}

	// Pulling CS high
	comm_interface.gpio_write(comm_interface.cs_gpio_port_handle, comm_interface.cs_pin, 1);

	return ret;
}




