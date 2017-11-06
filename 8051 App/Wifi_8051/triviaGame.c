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
void wifiConnect(char* type, char* IP, int port);
void startGame();

void main()
{
	initBoard();
	lightBlue();

	/*startGame();*/
	checkConnection();
	/*clearLcd();*/
	/*printf("This is a test... Testing ... 123");*/
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
	unsigned long count = 524280;
	int i = 0;

	/* Initial Start-Up Read*/
	wifiRead(524280, 1);
	delayXms(200);

	/*AT*/
	clearLcd();
	printf("AT");
	nextLine();
	wifiWrite("AT");
	wifiRead(174760, 1);
	delayXms(200);

	/*AT+CIPSTART*/
	clearLcd();
	printf("AT+CIPSTART");
	nextLine();
	wifiWrite("AT+CIPSTART=\"TCP\",\"18.221.30.192\",3000");
	wifiRead(174760, 1);
	delayXms(200);

	/*AT+CIPMODE*/
	clearLcd();
	printf("AT+CIPMODE");
	nextLine();
	wifiWrite("AT+CIPMODE=1");
	wifiRead(174760, 1);
	delayXms(200);

	/*AT+CIPSEND*/
	clearLcd();
	printf("AT+CIPSEND");
	nextLine();
	wifiWrite("AT+CIPSEND");
	wifiRead(174760, 1);
	delayXms(200);

	/*Application*/
	/*Get the date and send to server*/
	/*clearLcd();
	writeString("Date: ");	/*Prompt for start address*/
	/*date = getKeys(8);
	nextLine();
	serverWriteX(date);
	wifiRead(524280);
	delayXms(500);*/

	startGame();

	/*End transparent transmission*/
	clearLcd();
	IOnM = 0;
	printf("+++");
	nextLine();
	serverWrite("+++");
	wifiRead(174760, 1);
	delayXms(1000);

	/*AT+CIPMODE*/
	clearLcd();
	printf("AT+CIPMODE");
	nextLine();
	wifiWrite("AT+CIPMODE=0");
	wifiRead(174760, 1);
	delayXms(200);

	/*AT+CIPCLOSE*/
	clearLcd();
	printf("AT+CIPCLOSE");
	nextLine();
	wifiWrite("AT+CIPCLOSE");
	wifiRead(174760, 1);

	while(1);
}

void wifiConnect(char* type, char* IP, int port)
{
	wifiWrite("AT+CIPSTART=\"TCP\",\"18.221.30.192\",3000" );
}

void startGame()
{
	int correct = 0;
	char startText[24] = "PRESS ANY KEY TO START!";
	char instructions[30] = "YOU'LL BE ASKED 10 QUESTIONS!";
	char __xdata* read;

	/*Display Menu*/
	clearLcd();
	IOnM = 0;
	writeString(startText);
	nextLine();
	writeString(instructions);
	getKey();

	/*Get questions*/
	clearLcd();
	serverWrite("GET");
	read = wifiRead(524280, 0);
	writeStringX(read);
	delayXms(2000);
}