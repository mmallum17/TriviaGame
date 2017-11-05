/***********************************************
* File: rtc.h
* Name: Marcus Mallum
* Date: 8/29/2017
* Desc: Provides different functions to control
*	LCD on the 8051 board
* Usage:
***********************************************/
#ifndef RTC_H
#define RTC_H

/*#include <8051.h>*/
/*#include "delay.h"*/
#define IOnM P3_5

__xdata unsigned char __at (0x0000) secondsOnes;
__xdata unsigned char __at (0x0001) secondsTens;
__xdata unsigned char __at (0x0002) minutesOnes;
__xdata unsigned char __at (0x0003) minutesTens;
__xdata unsigned char __at (0x0004) hoursOnes;
__xdata unsigned char __at (0x0005) hoursTens;
__xdata unsigned char __at (0x0006) daysOnes;
__xdata unsigned char __at (0x0007) daysTens;
__xdata unsigned char __at (0x0008) monthsOnes;
__xdata unsigned char __at (0x0009) monthsTens;
__xdata unsigned char __at (0x000A) yearsOnes;
__xdata unsigned char __at (0x000B) yearsTens;
__xdata unsigned char __at (0x000C) dayOfWeek;
__xdata unsigned char __at (0x000D) controlRegD;
__xdata unsigned char __at (0x000E) controlRegE;
__xdata unsigned char __at (0x000F) controlRegF;

void initRtc();
void checkStatus();
void showTime();


void showTime()
{
	unsigned char hours;
	unsigned char minutes;
	unsigned char seconds;
	unsigned char isPm;

	IOnM = 1;	/*Set to access IO*/

	writeCommand(0x80);	/*Go to first line of LCD*/

	/*Read hours, minutes, and seconds from RTC*/
	hours = (hoursOnes & 0x0F) + (hoursTens & 0x03) * 10;
	isPm = hoursTens & 0x04;
	minutes = (minutesOnes & 0x0F) + (minutesTens & 0x07) * 10;
	seconds = (secondsOnes & 0x0F) + (secondsTens & 0x07) * 10;

	/*Show Time*/
	writeData((hours / 10) | 0x30);
	writeData((hours % 10) | 0x30);
	writeData(':');
	writeData((minutes / 10) | 0x30);
	writeData((minutes % 10) | 0x30);
	writeData(':');
	writeData((seconds / 10) | 0x30);
	writeData((seconds % 10) | 0x30);
	writeData(' ');
	if(isPm)
	{
		writeString("PM");
	}
	else
	{
		writeString("AM");
	}
}

void initRtc()
{
	IOnM = 1;		/*Make IOnM indicate IO*/

	controlRegF = 0;	/*Iniitialize CF register*/
	controlRegD = 4;	/*Initialize CD register*/

	checkStatus();

	controlRegF = 3;	/*Stop and reset the counter*/

	/*Initialize the current time*/
	checkStatus();
	secondsOnes = 0;
	checkStatus();
	secondsTens = 0;
	checkStatus();
	minutesOnes = 0;
	checkStatus();
	minutesTens = 0;
	checkStatus();
	hoursOnes = 2;
	checkStatus();
	hoursTens = 1;
	checkStatus();
	daysOnes = 1;
	checkStatus();
	daysTens = 0;
	checkStatus();
	monthsOnes = 1;
	checkStatus();
	monthsTens = 0;
	checkStatus();
	yearsOnes = 7;
	checkStatus();
	yearsTens = 1;
	checkStatus();
	dayOfWeek = 0;

	controlRegF = 0;	/*Start Counters*/
	controlRegD = 4;	/*Start Counters*/
}

void checkStatus()
{
	unsigned char busyBit;
	IOnM = 1;	/*Set IOnM to access IO*/

	do
	{
		controlRegD = 5;	/*Set the hold bit*/
		busyBit = controlRegD & 2;	/*Read busy bit*/
		if(busyBit)
		{
			controlRegD = 4;	/*Clear the hold bit*/
			delayXus(250);		/*Cover 190us delay*/
		}
	}
	while(busyBit);
}

#endif