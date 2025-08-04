/*
 * magnetometer.c
 *
 *  Created on: Aug 4, 2025
 *      Author: arvyd
 */

#include <stdlib.h>
#include "magnetometer.h"

void light_up_led_towards_magnetic_field(Magnetometer_leds leds, LIS3MDL_Magnetic_Data_t magnetic_data){

	HAL_GPIO_WritePin(leds.neg_x_led_gpio_port, leds.neg_x_led_gpio_pin, 0);
	HAL_GPIO_WritePin(leds.pos_x_led_gpio_port, leds.pos_x_led_gpio_pin, 0);
	HAL_GPIO_WritePin(leds.neg_y_led_gpio_port, leds.neg_y_led_gpio_pin, 0);
	HAL_GPIO_WritePin(leds.pos_y_led_gpio_port, leds.pos_y_led_gpio_pin, 0);

	if(abs(magnetic_data.x) > abs(magnetic_data.y)){
		if (magnetic_data.x < 0){
			HAL_GPIO_WritePin(leds.neg_x_led_gpio_port, leds.neg_x_led_gpio_pin, 1);
			return;
		}
		HAL_GPIO_WritePin(leds.pos_x_led_gpio_port, leds.pos_x_led_gpio_pin, 1);
		return;
	}
	if (magnetic_data.y < 0){
		HAL_GPIO_WritePin(leds.neg_y_led_gpio_port, leds.neg_y_led_gpio_pin, 1);
		return;
	}
	HAL_GPIO_WritePin(leds.pos_y_led_gpio_port, leds.pos_y_led_gpio_pin, 1);
	return;
}


