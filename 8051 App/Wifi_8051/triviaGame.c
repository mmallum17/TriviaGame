/***********************************************
* File: wifi8051.c
* Name: Marcus Mallum
* Date: 10/06/2017
* Desc: Interface wifi and 8051
* Usage:
***********************************************/
/*#include <stdlib.h>*/
#include <ctype.h>
#include <stdio.h>
#include "adc.h"
#include "keypad.h"
#include "lcd.h"
#include "rtc.h"
#include "uart.h"
#include "wifi.h"

__xdata unsigned char __at (0xC000) sevenSeg;

void initBoard();
void checkConnection();
void wifiWrite(char* string);
void wifiConnect(char* type, char* IP, int port);
void wifiSend();

void main()
{
	initBoard();
	lightBlue();

	checkConnection();
	while(1);
}

void initBoard()
{
	initLCD();
	clearLcd();
	initRtc();
	UART_Init(2400);
}

void checkConnection()
{
	char ch;
	char key;
	char __xdata* date;
	/*char __xdata temp[4];*/
	unsigned long count = 524280;
	int i = 0;

	/* Initial Start-Up Read*/
	wifiRead(524280);
	delayXms(200);

	/*AT*/
	clearLcd();
	printf("AT");
	nextLine();
	wifiWrite("AT");
	wifiRead(524280);
	delayXms(200);

	/*AT+CIPSTART*/
	clearLcd();
	printf("AT+CIPSTART");
	nextLine();
	wifiWrite("AT+CIPSTART=\"TCP\",\"18.221.30.192\",3000");
	wifiRead(174760);
	delayXms(200);

	/*AT+CIPMODE*/
	clearLcd();
	printf("AT+CIPMODE");
	nextLine();
	wifiWrite("AT+CIPMODE=1");
	wifiRead(174760);
	delayXms(200);

	/*AT+CIPSEND*/
	clearLcd();
	printf("AT+CIPSEND");
	nextLine();
	wifiWrite("AT+CIPSEND");
	wifiRead(174760);
	delayXms(200);

	/*Get the date and send to server*/
	clearLcd();
	writeString("Date: ");	/*Prompt for start address*/
	date = getKeys(8);
	nextLine();
	serverWriteX(date);
	wifiRead(524280);
	delayXms(500);

	/*End transparent transmission*/
	clearLcd();
	IOnM = 0;
	printf("+++");
	nextLine();
	serverWrite("+++");
	wifiRead(174760);
	delayXms(1000);

	/*AT+CIPMODE*/
	clearLcd();
	printf("AT+CIPMODE");
	nextLine();
	wifiWrite("AT+CIPMODE=0");
	wifiRead(174760);
	delayXms(200);

	/*AT+CIPCLOSE*/
	clearLcd();
	printf("AT+CIPCLOSE");
	nextLine();
	wifiWrite("AT+CIPCLOSE");
	wifiRead(174760);

	while(1);
	/*delayXms(1000);*/
	/*while(count > 0)
	{
		if(RI==1)
		{
			ch = SBUF;
			RI = 0;
			printf("%c", ch);
		}
		count--;
		/*delayXms(250);*/
	/*}
	count = 524280;
	delayXms(500);
	clearLcd();
	printf("AT");
	nextLine();

	wifiWrite("AT");
	/*UART_TxChar('A');
	UART_TxChar('T');
	UART_TxChar(0xD);
	UART_TxChar(0xA);*/

	/*UART_TxChar(0x5C);
	UART_TxChar('r');
	UART_TxChar(0x5C);
	UART_TxChar('n');*/

	/*while(1)
	{*/
	/*while(count > 0)
	{
		if(RI==1)
		{
			ch = SBUF;
			RI = 0;
			printf("%c", ch);
		}
		count--;
		/*delayXms(250);*/
	/*}
	/*}*/
	/*printf("HERE");*/
	/*delayXms(500);
	clearLcd();
	printf("AT+CIPSTART");
	nextLine();

	wifiWrite("AT+CIPSTART=\"TCP\",\"18.221.30.192\",3000");
	count = 524280;
	while(count > 0)
	{
		if(RI==1)
		{
			ch = SBUF;
			RI = 0;
			printf("%c", ch);
		}
		count--;
		/*delayXms(250);*/
	/*}

	delayXms(500);
	clearLcd();
	printf("AT+CIPMODE");
	nextLine();

	wifiWrite("AT+CIPMODE=1");
	count = 524280;
	while(count > 0)
	{
		if(RI==1)
		{
			ch = SBUF;
			RI = 0;
			printf("%c", ch);
		}
		count--;
		/*delayXms(250);*/
	/*}

	delayXms(500);
	clearLcd();
	printf("AT+CIPSEND");
	nextLine();

	/*UART_TxChar('T');
	UART_TxChar('E');
	UART_TxChar('S');
	UART_TxChar('T');*/
	/*wifiWrite("AT+CIPSEND");
	count = 524280;
	while(count > 0)
	{
		if(RI==1)
		{
			ch = SBUF;
			RI = 0;
			printf("%c", ch);
		}
		count--;
		/*delayXms(250);*/
	/*}

	delayXms(500);
	clearLcd();

	writeString("Date: ");	/*Prompt for start address*/
	/*IOnM = 0;
	date[0] = getKey();
	key = date[0];
	writeData(key);
	IOnM = 0;
	date[1] = getKey();
	key = date[1];
	writeData(key);
	IOnM = 0;
	date[2] = getKey();
	key = date[2];
	writeData(key);
	IOnM = 0;
	date[3] = getKey();
	key = date[3];
	writeData(key);
	IOnM = 0;
	date[4] = getKey();
	key = date[4];
	writeData(key);
	IOnM = 0;
	date[5] = getKey();
	key = date[5];
	writeData(key);
	IOnM = 0;
	date[6] = getKey();
	key = date[6];
	writeData(key);
	IOnM = 0;
	date[7] = getKey();
	key = date[7];
	writeData(key);
	date[8] = 0;

	IOnM = 0;
	while(date[i] != 0)
	{
		UART_TxChar(date[i]);
		i++;
	}
	/*printf("TEST");*/
	/*nextLine();

	/*wifiWrite("AT+CIPMODE=1");*/
	/*UART_TxChar('2');
	UART_TxChar('0');
	UART_TxChar('1');
	UART_TxChar('7');
	UART_TxChar('0');
	UART_TxChar('1');
	UART_TxChar('0');
	UART_TxChar('1');*/
	/*count = 1048560;
	while(count > 0)
	{
		if(RI==1)
		{
			ch = SBUF;
			RI = 0;
			printf("%c", ch);
		}
		count--;
		/*delayXms(250);*/
	/*}

	delayXms(500);
	clearLcd();
	printf("+++");
	nextLine();

	UART_TxChar('+');
	UART_TxChar('+');
	UART_TxChar('+');
	/*wifiWrite("AT+CIPMODE=1");*/
	/*count = 524280;
	while(count > 0)
	{
		if(RI==1)
		{
			ch = SBUF;
			RI = 0;
			printf("%c", ch);
		}
		count--;
		/*delayXms(250);*/
	/*}

	delayXms(1000);
	clearLcd();
	printf("AT+CIPMODE");
	nextLine();

	wifiWrite("AT+CIPMODE=0");
	count = 524280;
	while(count > 0)
	{
		if(RI==1)
		{
			ch = SBUF;
			RI = 0;
			printf("%c", ch);
		}
		count--;
		/*delayXms(250);*/
	/*}

	delayXms(500);
	clearLcd();
	printf("AT+CIPCLOSE");
	nextLine();

	wifiWrite("AT+CIPCLOSE");
	count = 524280;
	while(count > 0)
	{
		if(RI==1)
		{
			ch = SBUF;
			RI = 0;
			printf("%c", ch);
		}
		count--;
		/*delayXms(250);*/
	/*}
	while(1);*/
}

void wifiConnect(char* type, char* IP, int port)
{
	wifiWrite("AT+CIPSTART=\"TCP\",\"18.221.30.192\",3000" );
}

/*void wifiWrite(char* string)
{
	while(*string)
	{
		UART_TxChar(*string++);
	}
	UART_TxChar(0xD);
	UART_TxChar(0xA);
}*/