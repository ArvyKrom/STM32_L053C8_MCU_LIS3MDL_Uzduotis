/*
 * lis3mdl_registers.h
 *
 *  Created on: Jul 31, 2025
 *      Author: arvyd
 */

#ifndef LIS3MDL_LIS3MDL_REGISTERS_H_
#define LIS3MDL_LIS3MDL_REGISTERS_H_

/*
 * Command bits
 */

#define LIS3MDL_READ_BIT 0x80
#define LIS3MDL_MD_BIT 0x40

/*
 * OFFSET Registers
 */

#define LIS3MDL_OFFSET_X_REG_L_M_ADDR 0x05
#define LIS3MDL_OFFSET_X_REG_H_M_ADDR 0x06
#define LIS3MDL_OFFSET_Y_REG_L_M_ADDR 0x07
#define LIS3MDL_OFFSET_Y_REG_H_M_ADDR 0x08
#define LIS3MDL_OFFSET_Z_REG_L_M_ADDR 0x09
#define LIS3MDL_OFFSET_Z_REG_H_M_ADDR 0x0A

/*
 * WHO_AM_I register
 */

#define LIS3MDL_WHO_AM_I_REG_ADDR 0x0F
#define LIS3MDL_WHO_AM_I_REG_VALUE 0x3D

/*
 * CTRL Registers and their bit masks
 */

#define LIS3MDL_CTRL_REG1_ADDR 0x20
#define LIS3MDL_TEMP_EN 0x80 // enables temeprature sensor
#define LIS3MDL_XY_OPERATING_MODE 0x60 // XY performance
// The higher the performance mode the more accurate readings will be
#define LIS3MDL_ODR 0x1C // For selecting outout data rate
#define LIS3MDL_FAST_ODR 0x2 // Turns Fast ODR mode
#define LIS3MDL_SELF_TEST 0x1

#define LIS3MDL_CTRL_REG2_ADDR 0x21

#define LIS3MDL_FULL_SCALE 0x60 // Selecting full scale
#define LIS3MDL_REBOOT 0x08 // Reboots memory content
#define LIS3MDL_SOFT_RST 0x04 // Resets all configuration and user registers

#define LIS3MDL_CTRL_REG3_ADDR 0x22
#define LIS3MDL_LP 0x20 // Puts the device to low power mode
#define LIS3MDL_SIM 0x04 // Changes SPI mode to 3 wire interface
#define LIS3MDL_MD 0x03 // Continous, single or idle mode

#define LIS3MDL_CTRL_REG4_ADDR 0x23
#define LIS3MDL_Z_OPERATING_MODE 0x0C // Z UltraHigh performance
#define LIS3MDL_BLE 0x2 // Switch up Output data MSb and LSb registers adresses

#define LIS3MDL_CTRL_REG5_ADDR 0x24
#define LIS3MDL_FAST_READ 0x80 // Enables fast read
#define LIS3MDL_BDU 0x40 // Output registers won't be updated until MSb and LSb are read

/*
 * STATUS register and it's bit masks
 */

#define LIS3MDL_STATUS_REG_ADDR 0x27
#define LIS3MDL_ZYXOR 0x80 // all axis data was overrun
#define LIS3MDL_ZOR 0x40 // Z axis data was overrun
#define LIS3MDL_YOR 0x20 // Y axis data was overrun
#define LIS3MDL_XOR 0x10 // X axis data was overrun
#define LIS3MDL_ZYXDA 0x08 // all axis new data is available
#define LIS3MDL_ZDA 0x04 // Z axis new data is available
#define LIS3MDL_YDA 0x02 // Y axis new data is available
#define LIS3MDL_XDA 0x01 // X axis new data is available

/*
 * OUTPUT registers
 */

#define LIS3MDL_OUT_X_L_ADDR 0x29
#define LIS3MDL_OUT_X_H_ADDR 0x29
#define LIS3MDL_OUT_Y_L_ADDR 0x2A
#define LIS3MDL_OUT_Y_H_ADDR 0x2B
#define LIS3MDL_OUT_Z_L_ADDR 0x2C
#define LIS3MDL_OUT_Z_H_ADDR 0x2D

/*
 * TEMP OUT registers
 */

#define LIS3MDL_TEMP_OUT_L_ADDR 0x2E
#define LIS3MDL_TEMP_OUT_H_ADDR 0x2F

/*
 * Interrupt registers and their bit masks
 */

#define LIS3MDL_INT_CFG_REG_ADDR 0x30
#define LIS3MDL_XIEN 0x80 // Enables interrupt generation from x axis
#define LIS3MDL_YIEN 0x40 // Enables interrupt generation from x axis
#define LIS3MDL_ZIEN 0x20 // Enables interrupt generation from x axis
#define LIS3MDL_IEA 0x04 // Enables interrupt active configuration
#define LIS3MDL_LIR 0x02 // Enables latch interrupt request
#define LIS3MDL_IEN 0x01 // Enables interrupt on int pin

#define LIS3MDL_INT_SRC_REG_ADDR 0x31
#define LIS3MDL_PTH_X 0x80 // value on X axis exceeds threshold on positive side
#define LIS3MDL_PTH_Y 0x40 // value on Y axis exceeds threshold on positive side
#define LIS3MDL_PTH_Z 0x20 // value on Z axis exceeds threshold on positive side
#define LIS3MDL_NTH_X 0x10 // value on X axis exceeds threshold on negative side
#define LIS3MDL_NTH_Y 0x08 // value on Y axis exceeds threshold on negative side
#define LIS3MDL_NTH_Z 0x04 // value on Z axis exceeds threshold on negative side
#define LIS3MDL_MROI 0x02 //internal measurement range overflow
#define LIS3MDL_INT 0x01 // interrupt was raised

#define LIS3MDL_INT_THS_L 0x32 // low size of the threshold (both positive and negative)
#define LIS3MDL_INT_THS_H 0x33 // high side of the threshold (both positive and negative)


#endif /* LIS3MDL_LIS3MDL_REGISTERS_H_ */
