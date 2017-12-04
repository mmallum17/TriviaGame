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
#include <stdlib.h>
#include <string.h>
#include "uart.h"

char __xdata* wifiRead(unsigned long timeout, int print, int serverRead, char* find, char __xdata* resp);
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

char __xdata* wifiRead(unsigned long timeout, int print, int serverRead, char* find, char __xdata* resp)
{
	char ch;
	/*char __xdata buffer[100] = "";*/
	char check[8] = {1, 1, 1, 1, 1, 1, 1, 0};
	char i = 0;
	char j = 0;


	IOnM = 0;
	/*buffer = (char*)calloc(1, sizeof(char));*/
	while(timeout > 0 && strstr(check, find/*"OK\r\n"*/)== NULL)
	{
		IOnM = 0;
		if(RI==1)
		{
			ch = SBUF;
			RI = 0;
			for(j = 0; j < 6; j++)
			{
				check[j] = check[j + 1];
			}
			check[6] = ch;
			if(serverRead)
			{
				resp[i++] = ch;
			}
			if(print)
			{
				printf("%c", ch);
				/*writeData(ch);*/
			}
		}
		/*if(find[0] == 't' && timeout % 2000 == 0)
		{
			printf("%lu ", timeout);
		}*/
		timeout--;
	}
	IOnM = 0;
	if(serverRead)
	{
		resp[i - 1] = 0;
	}
	return resp;
}

#endif