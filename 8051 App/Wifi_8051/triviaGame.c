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
#include "sevenSegLed.h"

__xdata unsigned char __at (0xC000) sevenSeg;

void initBoard();
void checkConnection();
void wifiConnect(char* type, char* IP, int port);
void startGame();

void main()
{
	initBoard();
	lightBlue();
	IOnM = 0;
	checkConnection();
	while(1);
}

void initBoard()
{
	initLed();
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
	wifiRead(5242, 1, 0, "IP\r\n", NULL);
	delayXms(500);

	/*AT*/
	clearLcd();
	printf("AT");
	nextLine();
	wifiWrite("AT");
	wifiRead(873, 1, 0, "OK\r\n", NULL);
	delayXms(500);

	/*AT+CIPSTART*/
	clearLcd();
	printf("AT+CIPSTART");
	nextLine();
	wifiWrite("AT+CIPSTART=\"TCP\",\"18.221.30.192\",3000");
	wifiRead(873, 1, 0, "OK\r\n", NULL);
	delayXms(500);

	/*AT+CIPMODE*/
	clearLcd();
	printf("AT+CIPMODE");
	nextLine();
	wifiWrite("AT+CIPMODE=1");
	wifiRead(873, 1, 0, "OK\r\n", NULL);
	delayXms(500);

	/*AT+CIPSEND*/
	clearLcd();
	printf("AT+CIPSEND");
	nextLine();
	wifiWrite("AT+CIPSEND");
	wifiRead(873, 1, 0, "OK\r\n\r\n>", NULL);
	delayXms(500);

	/*Application*/
	IOnM = 0;
	startGame();

	/*End transparent transmission*/
	clearLcd();
	IOnM = 0;
	printf("+++");
	nextLine();
	serverWrite("+++");
	/*wifiRead(873, 1, "trash");*/
	delayXms(1000);

	/*AT+CIPMODE*/
	clearLcd();
	printf("AT+CIPMODE");
	nextLine();
	wifiWrite("AT+CIPMODE=0");
	wifiRead(873, 1, 0, "OK\r\n", NULL);
	delayXms(500);

	/*AT+CIPCLOSE*/
	clearLcd();
	printf("AT+CIPCLOSE");
	nextLine();
	wifiWrite("AT+CIPCLOSE");
	wifiRead(873, 1, 0, "OK\r\n", NULL);

	while(1);
}

void wifiConnect(char* type, char* IP, int port)
{
	wifiWrite("AT+CIPSTART=\"TCP\",\"18.221.30.192\",3000" );
}

void startGame()
{
	int i = 0;
	char __xdata startText[24] = "PRESS ANY KEY TO START!";
	char __xdata instructions[28] = "TRY TO ANSWER 10 QUESTIONS!";
	char __xdata read[100] = "";
	int j = 0;
	char correctChoice = 0;
	char __xdata answer[20] = "";
	char delimiter[2] = "\r";
	char choice = 0;
	int score = 0;

	/*Display Menu*/
	clearLcd();
	IOnM = 0;
	writeStringX(startText);
	nextLine();
	writeStringX(instructions);
	getKey(10);

	for(i = 0; i < 10; i++)
	{
		/*Get questions*/
		clearLcd();
		serverWrite("GETQ");
		IOnM = 0;
		wifiRead(1747, 0, 1, "\x1A", read);
	 	writeStringX(read);
	 	delayXms(2000);

	 	/*Get Answers*/
	 	clearLcd();
	 	serverWrite("GETC");
	 	IOnM = 0;
	 	wifiRead(1747, 0, 1, "\x1A", read);
	 	correctChoice = read[0] - 48;

	 	serverWrite("GET0");
	 	IOnM = 0;
	 	wifiRead(1747, 0, 1, "\x1A", read);
	 	toLine(1);
	 	printf("A. ");
	 	writeStringX(read);

	 	serverWrite("GET1");
	 	IOnM = 0;
	 	wifiRead(1747, 0, 1, "\x1A", read);
	 	toLine(2);
	 	printf("B. ");
	 	writeStringX(read);

	 	serverWrite("GET2");
	 	IOnM = 0;
	 	wifiRead(1747, 0, 1, "\x1A", read);
	 	toLine(3);
	 	printf("C. ");
	 	writeStringX(read);

	 	serverWrite("GET3");
	 	IOnM = 0;
	 	wifiRead(1747, 0, 1, "\x1A", read);
	 	toLine(4);
	 	printf("D. ");
	 	writeStringX(read);

		choice = 0;
		for(j = 11; j > 1 && !choice; j--)
		{
			lightNumber(j - 2);
	 		choice = getKey(1);
	 	}

	 	choice -= 65;
	 	clearLcd();
	 	if(choice == correctChoice)
	 	{
	 		printf("CORRECT!!");
	 		score += j;
	 		lightGreen();
	 	}
	 	else
	 	{
	 		printf("INCORRECT!!");
	 		lightRed();
	 	}
	 	nextLine();
	 	printf("Score: %d", score);
	 	delayXms(2000);
	 	lightBlue();
	}
}