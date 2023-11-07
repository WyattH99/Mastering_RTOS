/*
 * rtc.c
 *
 *  Created on: Nov 1, 2023
 *      Author: wyatt
 */

#include "main.h"


void show_time_date(void){

	static char showdate[40];
	static char showtime[40];

	RTC_DateTypeDef rtc_date;
	RTC_TimeTypeDef rtc_time;

	static char* date = showdate;
	static char* time = showtime;

	memset(&rtc_date, 0, sizeof(rtc_date));
	memset(&rtc_time, 0, sizeof(rtc_time));

	// Get the RTC Current Date and Time
	HAL_RTC_GetDate(&hrtc, &rtc_date, RTC_FORMAT_BIN);
	HAL_RTC_GetTime(&hrtc, &rtc_time, RTC_FORMAT_BIN);

	char* format;
	format = (rtc_time.TimeFormat == RTC_HOURFORMAT12_AM) ? "AM" : "PM";

	//Display Date Format : date-month-year
	sprintf((char*)showdate, "\t%02d-%02d-%02d\n", rtc_date.Month, rtc_date.Date, 2000 + rtc_date.Year);
	xQueueSend(q_print_handle, &date, portMAX_DELAY);

	// Display time Format : hh:mm:ss [AM/PM]
	sprintf((char*)showtime, "%s: \t%02d:%02d:%02d [%s]", "\nCurrent Time&Date", rtc_time.Hours, rtc_time.Minutes, rtc_time.Seconds, format);
	xQueueSend(q_print_handle, &time, portMAX_DELAY);

}



void rtc_configure_time(RTC_TimeTypeDef* time){

	time->TimeFormat = RTC_HOURFORMAT12_AM;

	HAL_RTC_SetTime(&hrtc, time, RTC_FORMAT_BIN);

}


void rtc_configure_date(RTC_DateTypeDef* date){

	HAL_RTC_SetDate(&hrtc, date, RTC_FORMAT_BIN);

}





