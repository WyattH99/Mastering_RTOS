/*
 * led_effect.c
 *
 *  Created on: Nov 1, 2023
 *      Author: wyatt
 */

#include "main.h"


void led_effect_stop(void){

	for(int i=0; i<4; i++){
		xTimerStop(led_timer_handle[i], portMAX_DELAY);
	}

}


void led_effect(int n){

	led_effect_stop();
	xTimerStart(led_timer_handle[n-1], portMAX_DELAY);

}


void turn_off_all_leds(void){

	HAL_GPIO_WritePin(LD3_GPIO_Port, RED1_LED, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LD4_GPIO_Port, RED2_LED, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LD5_GPIO_Port, GREEN1_LED, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LD6_GPIO_Port, GREEN2_LED, GPIO_PIN_RESET);

}


void turn_on_all_leds(void){

	HAL_GPIO_WritePin(LD3_GPIO_Port, RED1_LED, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LD4_GPIO_Port, RED2_LED, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LD5_GPIO_Port, GREEN1_LED, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LD6_GPIO_Port, GREEN2_LED, GPIO_PIN_SET);

}


void turn_on_even_leds(void){

	HAL_GPIO_WritePin(LD3_GPIO_Port, RED1_LED, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LD4_GPIO_Port, RED2_LED, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LD5_GPIO_Port, GREEN1_LED, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LD6_GPIO_Port, GREEN2_LED, GPIO_PIN_SET);

}


void turn_on_odd_leds(void){

	HAL_GPIO_WritePin(LD3_GPIO_Port, RED1_LED, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LD4_GPIO_Port, RED2_LED, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LD5_GPIO_Port, GREEN1_LED, GPIO_PIN_SET);
	HAL_GPIO_WritePin(LD6_GPIO_Port, GREEN2_LED, GPIO_PIN_RESET);

}


void LED_control(int val){
	// 2
	for(int i=0; i<4; i++){
		GPIO_PinState pinstate = ((val >> i)& 0x1);
		switch (i){
		case 0:
			HAL_GPIO_WritePin(LD3_GPIO_Port, RED1_LED, pinstate);
			break;
		case 1:
			HAL_GPIO_WritePin(LD4_GPIO_Port, RED2_LED, pinstate);
			break;
		case 2:
			HAL_GPIO_WritePin(LD5_GPIO_Port, GREEN1_LED, pinstate);
			break;
		case 3:
			HAL_GPIO_WritePin(LD6_GPIO_Port, GREEN2_LED, pinstate);
			break;

		}

	}

}


// Toggle all LEDs
void LED_effect1(void){

	static int flag = 1;
	(flag ^= 1) ? turn_off_all_leds() : turn_on_all_leds();

}


// Toggle even and odd LEDs
void LED_effect2(void){

	static int flag = 1;
	(flag ^= 1) ? turn_on_even_leds() : turn_on_odd_leds();

}


// Count up LEDs
void LED_effect3(void){

	static int i = 0;
	LED_control( 0x1 << (i++ % 4));

}


// Count down LEDs
void LED_effect4(void){

	static int i = 0;
	LED_control( 0x08 >> (i++ % 4));

}
