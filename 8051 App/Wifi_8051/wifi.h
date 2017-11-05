/***********************************************
* File: wifi.h
* Name: Marcus Mallum
* Date: 10/23/2017
* Desc: Provides wifi functions for 8051
* Usage:
***********************************************/
#ifndef WIFI_H
#define WIFI_H

#include <8051.h>

void wifiRead(unsigned long timeout);
void wifiWrite(char* string);
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

void serverWrite(char* string)
{
	while(*string)
	{
		UART_TxChar(*string++);
	}
}

void wifiRead(unsigned long timeout)
{
	char ch;

	while(timeout > 0)
		{
			if(RI==1)
			{
				ch = SBUF;
				RI = 0;
				printf("%c", ch);
			}
			timeout--;
}

#endif