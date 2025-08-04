/*
 * magnetometer.h
 *
 *  Created on: Aug 4, 2025
 *      Author: arvyd
 */

#ifndef INC_MAGNETOMETER_H_
#define INC_MAGNETOMETER_H_

#include "main.h"
#include "lis3mdl_device.h"

typedef struct{
	GPIO_TypeDef *pos_x_led_gpio_port;
	uint16_t pos_x_led_gpio_pin;
	GPIO_TypeDef *neg_x_led_gpio_port;
	uint16_t neg_x_led_gpio_pin;
	GPIO_TypeDef *pos_y_led_gpio_port;
	uint16_t pos_y_led_gpio_pin;
	GPIO_TypeDef *neg_y_led_gpio_port;
	uint16_t neg_y_led_gpio_pin;
}Magnetometer_leds;

void light_up_led_towards_magnetic_field(Magnetometer_leds leds, LIS3MDL_Magnetic_Data_t magnetic_data);

#endif /* INC_MAGNETOMETER_H_ */

