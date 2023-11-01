/*
 * task_handler.c
 *
 *  Created on: Nov 1, 2023
 *      Author: wyatt
 */

#include "main.h"

int extract_command(command_t* cmd);
void process_command(command_t* cmd);



void menu_task_handler(void* param){

	uint32_t cmd_addr;
	command_t* cmd;
	const char* msg_menu = "===================\n"
							"|		Menu		|\n"
							"===================\n"
							"LED Effect		----> 0\n"
							"Date and Time  ----> 1\n"
							"Exit			----> 2\n"
							"Enter your choice here: ";

	while(1){
		xQueueSend(q_print_handle, &msg_menu, portMAX_DELAY);

		xTaskNotifyWait(0, 0, &cmd_addr, portMAX_DELAY);

		cmd_addr = (command_t*)cmd_addr;
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
	}while(item != '\n');

	cmd->payload[i-1] = '\0';
	cmd->len = i-1;

	return 0;
}


void print_task_handler(void* param){
	while(1){

	}
}

void led_task_handler(void* param){
	while(1){

	}
}

void rtc_task_handler(void* param){
	while(1){

	}
}
