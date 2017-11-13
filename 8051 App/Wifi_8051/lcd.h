/***********************************************
* File: lcd.h
* Name: Marcus Mallum
* Date: 8/25/2017
* Desc: Provides different functions to control
*	LCD on the 8051 board
* Usage:
***********************************************/
#ifndef LCD_H
#define LCD_H

#include <8051.h>
#include <string.h>
#include <stdlib.h>
#include "delay.h"
#define IOnM P3_5
#define RW P3_4

__xdata unsigned char __at (0x8000) lcdCommand;
__xdata unsigned char __at (0x8001) lcdData;
__xdata unsigned char __at (0x2000) backlight;

void writeString(char *string);
void writeStringX(char __xdata* string);
void initLCD();
void writeCommand(unsigned char command);
void clearLcd();
unsigned char readBusyAndAddress();
void lightRed();
void lightGreen();
void lightBlue();
void nextLine();
void writeData(unsigned char data);
void putchar(char ch);
int onFirst();
int onSecond();
int onThird();
int onFourth();
void toLine(int line);

void initLCD()
{
	IOnM = 1;		/*Set for I/O*/
	RW = 0;			/*Clear for write*/
	delayXms(80);		/*Covers 40ms power up delay*/
	lcdCommand = 0x3C;	/*Function set (8-bit interface, 2 line, and 5x11 dot font)*/
	delayXus(60);		/*Covers 39us delay to execute function set*/
	lcdCommand = 0x3C;	/*Function set (8-bit interface, 2 lines, and 5x11 dot font)*/
	delayXus(60);		/*Covers 39us delay to execute function set*/
	writeCommand(0x0F);	/*Turn off display*/
	clearLcd();		/*Clear display*/
	writeCommand(0x06);	/*Entry mode set (have cursor move to left and disable shift)*/
	clearLcd();

	backlight = 7;		/*Turn off all backlights*/
}

void clearLcd()
{
	IOnM = 1;		/*Set to access I/O*/
	RW = 0;			/*Clear RW for write*/
	writeCommand(0x01);	/*Clear LCD*/
	writeCommand(0x80);
}

void writeCommand(unsigned char command)
{
	IOnM = 1;		/*Set to access I/O*/
	RW = 0;			/*Clear RW for write*/
	lcdCommand = command;	/*Set LCD=command*/
	readBusyAndAddress();		/*Wait for LCD to finish command*/
}

void writeData(unsigned char data)
{
	IOnM = 1;	/*Set to access I/O for LCD*/
	RW = 0;		/*Clear RW for write*/
	lcdData = data;	/*Send data to LCD*/
	readBusyAndAddress();	/*Wait for LCD to finish receiving data*/
}

void lightRed()
{
	IOnM = 1;	/*Set to access I/O for backlight*/
	backlight = 6;	/*Set backlight to blue*/
}

void lightGreen()
{
	IOnM = 1;	/*Set to access I/O for backlight*/
	backlight = 5;	/*Set backlight to blue*/
}

void lightBlue()
{
	IOnM = 1;	/*Set to access I/O for backlight*/
	backlight = 3;	/*Set backlight to blue*/
}

void writeString(char* string)
{
	int length;
	/*char test[40] = "HI THERE HELLO";*/
	unsigned int address;
	char delimiter[2] = " ";
	/*printf("%s", string);*/
	char* token = strtok(string, delimiter);

	/*printf("%s ", token);*/
	while(token != NULL)
	{
		length = strlen(token);
		address = readBusyAndAddress();
		if((onFirst() && length + address > 0x13) || (onSecond() && length + address > 0x53) || (onThird() && length + address > 0x27) || (onFourth() && length + address > 0x67))
		{
			nextLine();
		}
		printf("%s ", token);
		token = strtok(NULL, delimiter);
	}
}

void writeStringX(char __xdata* string)
{
	char localString[21] = "";
	/*char* localString;*/
	int length;
	int i;
	int mult = 0;
	char letter;
	int offset = 0;
	int j;

	IOnM = 0;
	length = strlen(string) + 1;
	/*localString = (char*)malloc(length * sizeof(char));*/
	/*printf("%d", length);*/
	/*nextLine();
	IOnM = 0;*/
	for(i = 0; i + 20 * mult - offset < length; i++)
	{
		IOnM = 0;
		letter = string[i + 20 * mult - offset];
		localString[i] = letter;
		/*If buffer full*/
		if(i == 19)
		{
			localString[20] = 0;
			if(string[i + 20 * mult + 1 - offset] != ' ')
			{
				for(j = 0; localString[19 - j] != ' '; j++)
				{
					localString[19 - j] = 0;
				}
				offset += j;
			}
			/*printf("%s", localString);*/
			writeString(localString);
			i = -1;
			mult++;
			delayXms(10);
		}
		/*printf("%c", localString[i]);*/
	}
	localString[i] = 0;
	/*for(i = 0; i < 8; i++)
	{
		writeData(localString[i]);
	}*/
	/*localString[8] = 0;*/
	/*letter = string[0];
	printf("%c", letter);
	IOnM = 0;
	letter = string[1];
	printf("%c", letter);
	IOnM = 0;
	letter = string[2];
	printf("%c", letter);
	IOnM = 0;
	letter = string[3];
	printf("%c", letter);
	IOnM = 0;
	letter = string[4];
	printf("%c", letter);*/
	/*localString = (char*)calloc(1, strlen(string));
	strcpy(localString, string);*/
	/*printf("%s", localString);*/
	/*free(localString);*/
	/*printf("%s", localString);*/
	writeString(localString);
}

unsigned char readBusyAndAddress()
{
	IOnM = 1;			/*Set to access I/O*/
	RW = 1;				/*Set RW to read LCD*/
	while(lcdCommand & 128);	/*Read LCD*/
	return lcdCommand;
}

void nextLine()
{
	/*unsigned char address = readBusyAndAddress();*/
	if(onFirst())		/*If on first line, go to second line*/
	{
		writeCommand(0xC0);
	}
	else if(onSecond())	/*If on second line go to third line*/
	{
		writeCommand(0x94);
	}
	else if(onThird())	/*If on third line go to fourth line*/
	{
		writeCommand(0xD4);
	}
	else	/*If on fourth line, clear LCD*/
	{
		/*delayXms(1000);*/
		clearLcd();
	}
}

void putchar(char ch)
{
	int address = readBusyAndAddress();
	int lastColumn = 0;
	if(address == 0x13 || address == 0x53 || address == 0x27 || address == 0x67)
	{
		lastColumn = 1;
	}
	writeData(ch);
	if(lastColumn)
	{
		if(onThird())
		{
			writeCommand(0xC0);
		}
		else if(onFourth())
		{
			writeCommand(0x94);
		}
		else if(onSecond())
		{
			writeCommand(0xD4);
		}
	}
}

int onFirst()
{
	int address = readBusyAndAddress();
	if(address >= 0x00 && address <= 0x13)
	{
		return 1;
	}
	return 0;
}

int onSecond()
{
	int address = readBusyAndAddress();
	if(address >= 0x40 && address <= 0x53)
	{
		return 1;
	}
	return 0;
}

int onThird()
{
	int address = readBusyAndAddress();
	if(address >= 0x14 && address <= 0x27)
	{
		return 1;
	}
	return 0;
}

int onFourth()
{
	int address = readBusyAndAddress();
	if(address >= 0x54 && address <= 0x67)
	{
		return 1;
	}
	return 0;
}

void toLine(int line)
{
	switch(line)
	{
		case 1:
			writeCommand(0x80);
			break;
		case 2:
			writeCommand(0xC0);
			break;
		case 3:
			writeCommand(0x94);
			break;
		case 4:
			writeCommand(0xD4);
			break;
	}
}
#endif