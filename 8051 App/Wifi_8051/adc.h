/***********************************************
* File: adc.h
* Name: Marcus Mallum
* Date: 8/29/2017
* Desc: Provides different functions to control
*	adc on the 8051 board
* Usage:
***********************************************/
#ifndef ADC_H
#define ADC_H

#include <8051.h>
#include "lcd.h"
#define IOnM P3_5
#define adcInt P3_2

__xdata unsigned char __at (0x4000) adc;

float getTemperature();
void showTemperature(float temperature);

float getTemperature()
{
	float rawVolts;
	float temperature;
	IOnM = 1;	/*Set to access IO*/
	adc = 0;	/*Send write pulse to ADC*/
	while(adcInt);	/*Wait for adcInt to go low*/
	rawVolts = (float)adc;
	temperature = (rawVolts / 256) * 5000;
	temperature = temperature / 10 - 50;
	/*writeData(((unsigned char)(temperature / 100)) | 0x30);
	temperature = ((unsigned int)temperature) % 100;
	writeData(((unsigned char)(temperature / 10)) | 0x30);
	temperature = ((unsigned int)temperature) % 10;
	writeData(((unsigned char)temperature) | 0x30);
	temperature = 0;*/
	/*rawVolts = (adc / 256) * 5000;	/*Read raw voltage (mv) from temp sensor*/
	/*temperature = (rawVolts / 10) - 50;	/*Convert raw voltage to temperature in celsius*/
	return temperature;
	/*MOV	DPTR, #4000H	; Location of ADC

		; Send write pulse to ADC
		MOV	A, #0
		MOVX	@DPTR, A	; Send write pulse to ADC
		;CLR	ADC_CS		; Enable ADC
		;CLR	ADC_RD		; Ready to read

WAIT:		JB	ADC_INT, WAIT	; Wait for data to be ready

		; Read Data from ADC
		MOVX	A, @DPTR	; Move raw data to A
		;SETB	ADC_CS		; End reading
		;SETB	ADC_RD		; End reading
		ACALL	CONVERT
		ACALL 	SHOW_TEMP
		ACALL	RAW_TO_C	; Convert raw data to celsius
		RET*/
}

void showTemperature(float temperature)
{
	IOnM = 1;
	writeData((((int)temperature) / 10) | 0x30);
	writeData((((int)temperature) % 10) | 0x30);
	writeData(0xDF);
	writeData(' ');
	writeData('C');
}

#endif