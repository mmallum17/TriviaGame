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
	IOnM = 0;
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
	wifiRead(5242, 1, 0, "IP\r\n");
	delayXms(500);

	/*AT*/
	clearLcd();
	printf("AT");
	nextLine();
	wifiWrite("AT");
	wifiRead(873, 1, 0, "OK\r\n");
	delayXms(500);

	/*AT+CIPSTART*/
	clearLcd();
	printf("AT+CIPSTART");
	nextLine();
	wifiWrite("AT+CIPSTART=\"TCP\",\"18.221.30.192\",3000");
	wifiRead(873, 1, 0, "OK\r\n");
	delayXms(500);

	/*AT+CIPMODE*/
	clearLcd();
	printf("AT+CIPMODE");
	nextLine();
	wifiWrite("AT+CIPMODE=1");
	wifiRead(873, 1, 0, "OK\r\n");
	delayXms(500);

	/*AT+CIPSEND*/
	clearLcd();
	printf("AT+CIPSEND");
	nextLine();
	wifiWrite("AT+CIPSEND");
	wifiRead(873, 1, 0, "OK\r\n\r\n>");
	delayXms(500);

	/*Application*/
	/*Get the date and send to server*/
	/*clearLcd();
	writeString("Date: ");	/*Prompt for start address*/
	/*date = getKeys(8);
	nextLine();
	serverWriteX(date);
	wifiRead(524280);
	delayXms(500);*/

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
	wifiRead(873, 1, 0, "OK\r\n");
	delayXms(500);

	/*AT+CIPCLOSE*/
	clearLcd();
	printf("AT+CIPCLOSE");
	nextLine();
	wifiWrite("AT+CIPCLOSE");
	wifiRead(873, 1, 0, "OK\r\n");

	while(1);
}

void wifiConnect(char* type, char* IP, int port)
{
	wifiWrite("AT+CIPSTART=\"TCP\",\"18.221.30.192\",3000" );
}

void startGame()
{
	int correct = 0;
	char __xdata startText[24] = "PRESS ANY KEY TO START!";
	char __xdata instructions[28] = "TRY TO ANSWER 10 QUESTIONS!";
	char __xdata* read;
	/*IOnM = 0;
	startText = (char __xdata*)malloc(9 * sizeof(char));*/
	/*char __xdata* instructions = ()char __xdata*)calloc(30, sizeof(char));*/

	/*startText[0] = 'H';
	startText[1] = 'I';
	startText[2] = ' ';
	startText[3] = 'T';
	startText[4] = 'H';
	startText[5] = 'E';
	startText[6] = 'R';
	startText[7] = 'E';
	startText[8] = 0;*/
	/*char __xdata* read;*/
	/*char letter;*/

	/*Display Menu*/
	clearLcd();
	IOnM = 0;
	writeStringX(startText);
	nextLine();
	writeStringX(instructions);
	getKey();

	/*Get questions*/
	clearLcd();
	serverWrite("GETQ");
	IOnM = 0;
 	read = wifiRead(1747, 0, 1, "\r");
 	/*read[strlen(read) - 1] = 0;*/
 	writeStringX(read);
 	delayXms(2000);

 	/*Get Answers*/
 	clearLcd();
	serverWrite("GETC");
	IOnM = 0;
 	read = wifiRead(873, 0, 1, "\r");
 	writeStringX(read);
 	/*delayXms(2000);*/

 	nextLine();
 	serverWrite("GETA");
 	IOnM = 0;
 	read = wifiRead(873, 0, 1, "\r");
 	writeStringX(read);
 	delayXms(2000);
	/*letter = read[0];
	printf("%c", letter);
	letter = read[1];
	printf("%c", letter);*/
	/*writeStringX(read);*/
	/*delayXms(2000);*/
}