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
#include "delay.h"
#define IOnM P3_5
#define RW P3_4

__xdata unsigned char __at (0x8000) lcdCommand;
__xdata unsigned char __at (0x8001) lcdData;
__xdata unsigned char __at (0x2000) backlight;

void writeString(unsigned char *string);
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

void initLCD()
{
	IOnM = 1;		/*Set for I/O*/
	RW = 0;			/*Clear for write*/
	delayXms(80);		/*Covers 40ms power up delay*/
	lcdCommand = 0x3C;	/*Function set (8-bit interface, 2 lines, and 5x11 dot font)*/
	delayXus(60);		/*Covers 39us delay to execute function set*/
	lcdCommand = 0x3C;	/*Function set (8-bit interface, 2 lines, and 5x11 dot font)*/
	delayXus(60);		/*Covers 39us delay to execute function set*/
	writeCommand(0x0F);	/*Turn off display*/
	clearLcd();		/*Clear display*/
	writeCommand(0x06);	/*Entry mode set (have cursor move to left and disable shift)*/

	backlight = 7;		/*Turn off all backlights*/
}

void clearLcd()
{
	IOnM = 1;		/*Set to access I/O*/
	RW = 0;			/*Clear RW for write*/
	writeCommand(0x01);	/*Clear LCD*/
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

void writeString(unsigned char *string)
{
	while(*string)
	{
		writeData(*string++);
	}
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
	unsigned char address = readBusyAndAddress();
	if(address <= 0x13)		/*If on first line, go to second line*/
	{
		writeCommand(0xC0);
	}
	else if(address >= 0x40 && address <= 0x53)	/*If on second line go to third line*/
	{
		writeCommand(0x94);
	}
	else if(address >= 0x14 && address <= 0x27)	/*If on third line go to fourth line*/
	{
		writeCommand(0xD4);
	}
	else	/*If on fourth line, clear LCD*/
	{
		clearLcd();
	}
}

void putchar(char ch)
{
	writeData(ch);
}

#endif