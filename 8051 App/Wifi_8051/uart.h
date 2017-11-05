/***********************************************
* File: uart.h
* Name: Marcus Mallum
* Date: 10/05/2017
* Desc: Provides different functions to control
	UART on the 8051
* Usage:
***********************************************/
#ifndef UART_H
#define UART_H

#include <8051.h>
#define IOnM P3_5

void UART_Init(int baudRate);
void UART_TxChar(char ch);
char UART_RxChar(void);

void UART_Init(int baudRate)
{
	SCON = 0x50;  /* Asynchronous mode, 8-bit data, 1-stop bit*/
	TMOD = 0x20;  /*Timer1 in Mode2.*/
	TH1 = 256 - (12000000UL)/(32*12*(long)baudRate); /* Load timer value for baudrate generation*/
	/*printf("%2X", (unsigned int)TH1);*/
	nextLine();
	TR1 = 1;      /*Turn ON the timer for Baud rate generation*/
}

void UART_TxChar(char ch)
{
	SBUF = ch;      /* Load the data to be transmitted*/
	while(TI==0);   /* Wait till the data is trasmitted*/
	TI = 0;         /*Clear the Tx flag for next cycle.*/
}

char UART_RxChar(void)
{
	while(RI==0);     /* Wait till the data is received*/
	RI = 0;           /* Clear Receive Interrupt Flag for next cycle*/
	return(SBUF);     /* return the received char*/
}

#endif