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
	const char* msg_menu =  "  ==================  \n"
							"||       Menu       ||\n"
							"  ==================  \n"
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
					// TODO: Exit

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


// Helper function
uint8_t getnumber(uint8_t* p, int len){
	int value;

	if(len > 1){
		value =  ((p[0] * 10) + (p[1] - 48));
	}else{
		value = p[0] - 48;
	}

	return value;
}

void rtc_task_handler(void* param){

	const char* msg_rtc_banner =
			"  ==================  \n"
			"||        RTC       ||\n"
			"  ==================  \n";
	const char* msg_rtc_choices =
			"Configure Time  ---> 0\n"
			"Configure Date  ---> 1\n"
			"Enable Reporting---> 2\n"
			"Exit            ---> 3\n"
			"Enter your choice here: ";

	const char* msg_rtc_hh = "Enter Hour (1-12): ";
	const char* msg_rtc_mm = "Enter Minutes (0-59): ";
	const char* msg_rtc_ss = "Enter Seconds (0-59): ";

	const char* msg_rtc_dd = "Enter Date (1-31): ";
	const char* msg_rtc_mo = "Enter Month (1-12): ";
	const char* msg_rtc_dow = "Enter Day (1-7 Sun:1): ";
	const char* msg_rtc_yr = "Enter Year (0-99): ";

	const char* msg_conf = "Configuration Successful\n";
	const char* msg_rtc_report = "Enable Time & Date Reporting (y/n)?: ";

	uint32_t cmd_addr;
	command_t* cmd;

	static int rtc_config_state = 0;
	int menu_code;
	int option;

#define HH_CONFIG 0
#define MM_CONFIG 1
#define SS_CONFIG 2

#define DATE_CONFIG  0
#define MONTH_CONFIG 1
#define YEAR_CONFIG  2
#define DAY_CONFIG   3

	RTC_TimeTypeDef time;
	RTC_DateTypeDef date;

	while(1){

		//TODO: Notify Wait until Task is notified
		xTaskNotifyWait(0, 0, NULL, portMAX_DELAY);

		//TODO: Print the RTC menu and show current date and time information
		xQueueSend(q_print_handle, &msg_rtc_banner, portMAX_DELAY);
		show_time_date();
		xQueueSend(q_print_handle, &msg_rtc_choices, portMAX_DELAY);


		while(curr_state != sMainMenu){

			//TODO: Notify Wait
			xTaskNotifyWait(0, 0, &cmd_addr, portMAX_DELAY);
			cmd = (command_t*)cmd_addr;


			switch(curr_state){
				case sRtcMenu:{
					//TODO: process RTC menu commands
					if(cmd->len == 1){
						// Convert ASCI to Number
						option = cmd->payload[0] - 48;
						switch(option){
							case 0:{
								// Configure Time
								curr_state = sRtcTimeConfig;
								xQueueSend(q_print_handle, &msg_rtc_hh, portMAX_DELAY);
								break;
							}
							case 1:{
								// Configure Date
								curr_state = sRtcDateConfig;
								xQueueSend(q_print_handle, &msg_rtc_dd, portMAX_DELAY);
								break;
							}
							case 2:{
								// Enable Reporting
								curr_state = sRtcReport;
								xQueueSend(q_print_handle, &msg_rtc_report, portMAX_DELAY);
								break;
							}
							case 3:{
								// TODO: Exit
								curr_state = sMainMenu;
								break;
							}
							default:
								curr_state = sMainMenu;
								xQueueSend(q_print_handle, &msg_inv, portMAX_DELAY);
						}

					}else{
						// Invalid entry
						curr_state = sMainMenu;
						xQueueSend(q_print_handle, &msg_inv, portMAX_DELAY);
						continue;

					}

					break;
				}
				case sRtcTimeConfig:{
					//TODO: get hh, mm , ss information and configure RTC
					switch(rtc_config_state){
						case HH_CONFIG:{
							uint8_t hour = getnumber(cmd->payload, cmd->len);
							time.Hours = hour;
							rtc_config_state = MM_CONFIG;
							xQueueSend(q_print_handle, &msg_rtc_mm, portMAX_DELAY);
							break;}
						case MM_CONFIG:{
							uint8_t minute = getnumber(cmd->payload, cmd->len);
							time.Minutes = minute;
							rtc_config_state = SS_CONFIG;
							xQueueSend(q_print_handle, &msg_rtc_ss, portMAX_DELAY);
							break;}
						case SS_CONFIG:{
							uint8_t second = getnumber(cmd->payload, cmd->len);
							time.Seconds = second;
							if(validate_rtc_information(&time, NULL) == 1){
								rtc_configure_time(&time);
								xQueueSend(q_print_handle, &msg_conf, portMAX_DELAY);
								show_time_date();
							}else{
								xQueueSend(q_print_handle, &msg_inv, portMAX_DELAY);
							}
							curr_state = sMainMenu;
							rtc_config_state = 0;
							break;}
					}

					break;
				}
				case sRtcDateConfig:{
					//TODO: get date, month, day, year information and configure RTC
					switch(rtc_config_state){
						case DATE_CONFIG:{
							uint8_t dateTemp = getnumber(cmd->payload, cmd->len);
							date.Date = dateTemp;
							rtc_config_state = MONTH_CONFIG;
							xQueueSend(q_print_handle, &msg_rtc_mo, portMAX_DELAY);
							break;
						}
						case MONTH_CONFIG:{
							uint8_t month = getnumber(cmd->payload, cmd->len);
							date.Month = month;
							rtc_config_state = DAY_CONFIG;
							xQueueSend(q_print_handle, &msg_rtc_dow, portMAX_DELAY);
							break;
						}
						case DAY_CONFIG:{
							uint8_t day = getnumber(cmd->payload, cmd->len);
							date.WeekDay = day;
							rtc_config_state = YEAR_CONFIG;
							xQueueSend(q_print_handle, &msg_rtc_yr, portMAX_DELAY);
							break;
						}
						case YEAR_CONFIG:{
							uint8_t year = getnumber(cmd->payload, cmd->len);
							date.Year = year;
							if(validate_rtc_information(NULL, &date) == 1){
								rtc_configure_date(&date);
								xQueueSend(q_print_handle, &msg_conf, portMAX_DELAY);
								show_time_date();
							}else{
								xQueueSend(q_print_handle, &msg_inv, portMAX_DELAY);
							}
							curr_state = sMainMenu;
							rtc_config_state = 0;
							break;
						}
					}

					break;
				}
				case sRtcReport:{
					//TODO: enable or disable RTC current time reporting over ITM printf

					break;
				}
				default:{
					continue;
				}

			} // Curr_state Switch End
		} // While End

		//TODO: Notify menu task
		xTaskNotify(menu_task_handle, 0, eNoAction);

	} // Super While End
}
