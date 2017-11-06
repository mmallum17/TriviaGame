/***********************************************
* File: wifi.h
* Name: Marcus Mallum
* Date: 10/23/2017
* Desc: Provides wifi functions for 8051
* Usage:
***********************************************/
#ifndef WIFI_H
#define WIFI_H

#define IOnM P3_5

#include <8051.h>
#include<stdlib.h>
#include "uart.h"

char __xdata* wifiRead(unsigned long timeout, int print);
void wifiWrite(char* string);
void serverWriteX(char __xdata* string);
void serverWrite(char* string);

void wifiWrite(char* string)
{
	while(*string)
	{
		UART_TxChar(*string++);
	}
	UART_TxChar(0xD);
	UART_TxChar(0xA);
}

void serverWriteX(char __xdata* string)
{
	IOnM = 0;
	while(*string)
	{
		UART_TxChar(*string++);
	}
}

void serverWrite(char* string)
{
	IOnM = 0;
	while(*string)
	{
		UART_TxChar(*string++);
	}
}

char __xdata* wifiRead(unsigned long timeout, int print)
{
	char ch;
	char __xdata buffer[100] = "";
	int i = 0;


	IOnM = 0;
	/*buffer = (char*)calloc(1, sizeof(char));*/
	while(timeout > 0)
	{
		IOnM = 0;
		if(RI==1)
		{
			ch = SBUF;
			RI = 0;
			buffer[i++] = ch;
			if(print)
			{
				printf("%c", ch);
			}
		}
		timeout--;
	}
	IOnM = 0;
	return buffer;
}

#endif