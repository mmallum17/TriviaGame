/***********************************************
* File: delay.h
* Name: Marcus Mallum
* Date: 8/26/2017
* Desc: Provides delays for the 8051 board
* Usage:
***********************************************/
#ifndef DELAY_H
#define DELAY_H

void delay1ms();
void delayXms(unsigned int x);
void delayXus(unsigned int x);

void delayXms(unsigned int x)
{
	for(; x > 0; x--)
	{
		delay1ms();
	}
}

void delay1ms()
{
	__asm
			PUSH 	0		; Preserve register contents
			PUSH 	1
			MOV	R0, #5
		REP1:	MOV	R1, #250	; 5x255x1us = 1ms
		REP2:	DJNZ	R1, REP2	; If first counter not done, keep counting
			DJNZ	R0, REP1	; If second counter not done, keep counting
			POP	1		; Preserve register contents
			POP	0
	__endasm;
}

void delayXus(unsigned int x)
{
	for(; x != 0; x--)
	{
		__asm
			NOP
		__endasm;
	}
}

#endif