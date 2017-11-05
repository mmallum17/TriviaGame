/***********************************************
* File: keypad.h
* Name: Marcus Mallum
* Date: 8/27/2017
* Desc: Provides different functions to control
*	keypad on the 8051 board
* Usage:
***********************************************/
#ifndef KEYPAD_H
#define KEYPAD_H

#define IOnM P3_5

unsigned char getKey();
char __xdata* getKeys(unsigned int number);

unsigned char getKey()
{
		unsigned char rawKey;
		do
		{
			P1 = 0xF0;		/*Ground all rows at once*/
			rawKey = P1 & 0xF0; 	/*Read all columns, ensure all keys open, masking unused bits*/
		}while(rawKey != 0xF0);	/*Check till all keys released*/
		do
		{
			P1 = 0xF0;		/*Ground all rows at once*/
			delayXms(20);		/*Debouncing delay*/
			rawKey = P1 & 0xF0;	/*See if any key is pressed and mask unused bits*/
			if(rawKey != 0xF0)
			{
				delayXms(20);		/*Debouncing delay*/
				rawKey = P1 & 0xF0;	/*See if key is still pressed and mask unused bits*/
				if(rawKey != 0xF0)	/*Key has been pressed*/
				{
					P1 = 0xFE;		/*Ground row 0*/
					rawKey = P1 & 0xF0;	/*Read all columns and mask unused bits*/
					if(rawKey != 0xF0)	/*Key is in row 0*/
					{
						switch(rawKey)
						{
							case 0xE0:
								return '1';
							case 0xD0:
								return '2';
							case 0xB0:
								return '3';
							case 0x70:
								return 'A';
						}
					}
					P1 = 0xFD;		/*Ground row 1*/
					rawKey = P1 & 0xF0;	/*Read all columns and mask unused bits*/
					if(rawKey != 0xF0)	/*Key is in row 1*/
					{
						switch(rawKey)
						{
							case 0xE0:
								return '4';
							case 0xD0:
								return '5';
							case 0xB0:
								return '6';
							case 0x70:
								return 'B';
						}
					}
					P1 = 0xFB;		/*Ground row 2*/
					rawKey = P1 & 0xF0;	/*Read all columns and mask unused bits*/
					if(rawKey != 0xF0)	/*Key is in row 2*/
					{
						switch(rawKey)
						{
							case 0xE0:
								return '7';
							case 0xD0:
								return '8';
							case 0xB0:
								return '9';
							case 0x70:
								return 'C';
						}
					}
					P1 = 0xF7;		/*Ground row 3*/
					rawKey = P1 & 0xF0;	/*Read all columns and mask unused bits*/
					if(rawKey != 0xF0)	/*Key is in row 3*/
					{
						switch(rawKey)
						{
							case 0xE0:
								return 'F';
							case 0xD0:
								return '0';
							case 0xB0:
								return 'E';
							case 0x70:
								return 'D';
						}
					}
				}
			}
		}while(1);	/*Wait for key to be pressed*/
}

char __xdata* getKeys(unsigned int number)
{
	char __xdata input[11] = "";
	unsigned int i;
	char key;

	for(i = 0; i < number; i++)
	{
		IOnM = 0;
		input[i] = getKey();
		key = input[i];
		writeData(key);
	}
	IOnM = 0;
	input[i] = 0;
	return input;
}

#endif