/*
 * task_handler.c
 *
 *  Created on: Nov 1, 2023
 *      Author: wyatt
 */

#include "main.h"

int extract_command(command_t* cmd);
void process_command(command_t* cmd);

const char* msg_inv = "//////Invalid Option//////\n";


void menu_task_handler(void* param){

	uint32_t cmd_addr;
	command_t* cmd;
	int option;
	const char* msg_menu = "  =================== \n"
							"||       Menu       ||\n"
							"  =================== \n"
							"LED Effect    ----> 0\n"
							"Date and Time ----> 1\n"
							"Exit          ----> 2\n"
							"Enter your choice here: ";

	while(1){
		// Send message to the menu
		xQueueSend(q_print_handle, &msg_menu, portMAX_DELAY);

		// Wait for a response and put it into the cmd_addr
		xTaskNotifyWait(0, 0, &cmd_addr, portMAX_DELAY);

		// Cast it to a command_t pointer and assign the address to cmd
		cmd = (command_t*)cmd_addr;

		// Ensure the cmd is a valid entry of length 1. Valid Commands: 0, 1, or 2
		if(cmd->len == 1){

			// Convert the ASCI to number by subtracting 48
			option = cmd->payload[0] - 48;

			switch(option){
			case 0:
				// Led Effects
				curr_state = sLedEffect;
				xTaskNotify(led_task_handle, 0, eNoAction);
				break;
			case 1:
				// RTC Menu
				curr_state = sRtcMenu;
				xTaskNotify(rtc_task_handle, 0, eNoAction);
				break;
			case 2:
				// Exit

				break;
			default:
				xQueueSend(q_print_handle, &msg_inv, portMAX_DELAY);
				continue;
			}

		}else{

			// Invalid Entry
			xQueueSend(q_print_handle, &msg_inv, portMAX_DELAY);
			continue;

		}

		xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);

	}
}


void cmd_task_handler(void* param){

	BaseType_t returnedVal;
	command_t command;

	while(1){

		// Notify wait until notification
		returnedVal = xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);
		// Process the user data(command) stored in input data queue
		if(returnedVal == pdTRUE){
			process_command(&command);
		}



		// Notify the command to relevant task
	}
}

// Helper function for Command
void process_command(command_t* cmd){

	extract_command(cmd);

	switch(curr_state){
	case sMainMenu:
		xTaskNotify(menu_task_handle, (uint32_t)cmd, eSetValueWithOverwrite);
		break;
	case sLedEffect:
		xTaskNotify(led_task_handle, (uint32_t)cmd, eSetValueWithOverwrite);
		break;
	case sRtcMenu:
	case sRtcTimeConfig:
	case sRtcDateConfig:
	case sRtcReport:
		xTaskNotify(rtc_task_handle, (uint32_t)cmd, eSetValueWithOverwrite);
		break;
	}
}

// Helper Function for Process_Command
int extract_command(command_t* cmd){

	uint8_t item;
	BaseType_t status;

	status = uxQueueMessagesWaiting(q_data_handle);
	if(!status) return -1;
	uint8_t i = 0;

	do{
		status = xQueueReceive(q_data_handle, &item, 0);
		if(status == pdTRUE) cmd->payload[i++] = item;
	}while(item != '\r');

	cmd->payload[i-1] = '\0';
	cmd->len = i-1;

	return 0;
}


void print_task_handler(void* param){

	uint32_t* msg;
	while(1){
		xQueueReceive(q_print_handle, &msg, portMAX_DELAY);
		HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen((char*)msg), HAL_MAX_DELAY);

	}
}

void led_task_handler(void* param){

	uint32_t cmd_addr;
	command_t* cmd;
	const char* msg_led = "  =================== \n"
							"||   LED Effects   ||\n"
							"  =================== \n"
							"(none, e1, e2, e3, e4)\n"
							"Enter your choice here: ";

	while(1){

		// TODO: Wait for notification (Notify Wait)
		xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);

		// TODO: Print LED menu
		xQueueSend(q_print_handle, &msg_led, portMAX_DELAY);

		// TODO: wait for LED commands (Notify Wait)
		xTaskNotifyWait(0, 0, &cmd_addr, portMAX_DELAY);

		cmd = (command_t*)cmd_addr;

		if(cmd->len <= 4){
			if(! strcmp((char*)cmd->payload, "none")){
				led_effect_stop();
			}else if(! strcmp((char*)cmd->payload, "e1")){
				led_effect(1);
			}else if(! strcmp((char*)cmd->payload, "e2")){
				led_effect(2);
			}else if(! strcmp((char*)cmd->payload, "e3")){
				led_effect(3);
			}else if(! strcmp((char*)cmd->payload, "e4")){
				led_effect(4);
			}else{
				xQueueSend(q_print_handle, &msg_inv, portMAX_DELAY);
			}
		}else{
			// TODO: print Invalid Message
			xQueueSend(q_print_handle, &msg_inv, portMAX_DELAY);
		}

		// TODO: update state variable
		curr_state = sMainMenu;

		// TODO: Notify menu task
		xTaskNotify(menu_task_handle, 0, eNoAction);


	}
}

void rtc_task_handler(void* param){

//	const char* msg_test = "+";

	while(1){
		xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);

		//xQueueSend(q_print_handle, &msg_test, portMAX_DELAY);

	}
}
