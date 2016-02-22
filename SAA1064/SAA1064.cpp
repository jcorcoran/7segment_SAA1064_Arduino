/*
 *  SAA1064.cpp
 *  
 *
 *  Created by Christopher Parish on 19/01/2010.
 *  Copyright 2010 Chris Parish. All rights reserved.
 *
 */
#include <stdio.h>
#include <string.h>
#include "Wire.h"
#include "SAA1064.h"
#include <inttypes.h>

/**
 * Constructor
 *
 * @value	int addr	The address of the SAA1064 controler
 */
SAA1064::SAA1064(int addr)
{
	_reverse_digit_order = false;
	_address = addr >> 1; //convert the real address to an arduino compatable address
	_command = BRIGHT_MED & ~MULTIPLEX;
	_digits = 2;
	_dp = 0;
	Wire.begin();
}

/**
 * Clears the display by sending zeros
 */
void SAA1064::clear()
{
	Wire.beginTransmission(_address);
	Wire.write(1);
	for (int i=1; i <= _digits; i++) {
		Wire.write(0x00);
	}
	Wire.endTransmission();
}

/**
 * Reverses the order digits are written out to the display.
 * Default is low to high, pass true in to output digits in high to low order.
 */
void SAA1064::digit_order(bool reversed)
{
	_reverse_digit_order = reversed;
}

/**
 * Turns on multiplexing
 */
void SAA1064::multiplexOn()
{
	_command |= MULTIPLEX;
	_digits = 4;
}

/**
 * Turns off multiplexing
 */
void SAA1064::multiplexOff()
{
	_command &= ~MULTIPLEX;
	_digits = 2;
}

/**
 * Sets the brightness of the display by changing the resistance of the sink
 *
 * @value	char	brightness	Can be "hi" "med" and "low"
 */
void SAA1064::setBrightness(char *brightness)
{
	//clear all the brightness bits
	_command &= B10000111;
	if (strcmp(brightness,"hi") == 0) {
		_command |= BRIGHT_HI;
	} else if (strcmp(brightness,"med") == 0) {
		_command |= BRIGHT_MED;
	} else {
		_command |= BRIGHT_LOW;
	}
	sendCommand();
}

/**
 * Turns all the segments on for test purposes
 */
void SAA1064::test()
{
	Wire.beginTransmission(_address);
	Wire.write(0);
	Wire.write(_command | TEST_ALL);
	Wire.endTransmission();
	delay (1000);
	Wire.beginTransmission(_address);
	Wire.write(0);
	Wire.write(_command);
	Wire.endTransmission();
}

/**
 * Sets the number of decimal places to use
 */
void SAA1064::setDP(int point)
{
	if (point <= 4 && point >= 0) {
		_dp = point;
	} else {
		_dp = 0;
	}
}

/**
 * Sends an integer value to the display. Doesn't take into account the decimal
 * point
 *
 * @var		int	num	The number to display
 */
void SAA1064::send(int num)
{
	updateLED((long)num);
}

/**
 * Sends a long integer number to the display. takes into account the decimal place
 * and changes the value of num accoringly.
 *
 * @var		long	num	The number to display
 */
void SAA1064::send(long num)
{
	num = num * (pow (10, _dp));
	updateLED(num);
}

/**
 * Sends a float (real) number to the display, taking into account the number of
 * decimal places
 *
 * note: don't set the number of decimal places unnesseserily high, as it will
 * 	 case abnormal effects
 *
 * @var 	float	num	The number to display
 */
void SAA1064::send(float num)
{
	num = num * (pow (10, _dp));
	num = (uint16_t)num;
	updateLED(num);
}

/**
 * Sends a hex value to the display. The passed value an be in any integer format
 *
 * @var		uint16_t	num	The number to be displayed as hex
 */
void SAA1064::sendHex(uint16_t num)
{
	int a[32];
	int i=0;
	num = num % 0x10000;
	while( num != 0 && i < 32 )
	{
		a[i] = num % 16;
		num = num/16;
		i++;
	}
	if (i > 4) {
		i = 4;
	}
	int bytes[4] = {0,0,0,0};
	for (int loop = i-1; loop >= 0; loop--)
	{
		bytes[loop] = numbers[a[loop]];
		Serial.println(loop);
	}
	Wire.beginTransmission(_address);
	Wire.write(1);
	if (_command & MULTIPLEX) {
		if(_reverse_digit_order) {
			Wire.write(bytes[3]);
			Wire.write(bytes[2]);
			Wire.write(bytes[1]);
			Wire.write(bytes[0]);

		} else {
			Wire.write(bytes[0]);
			Wire.write(bytes[1]);
			Wire.write(bytes[2]);
			Wire.write(bytes[3]);
		}
	} else {
		if(_reverse_digit_order) {
			Wire.write(bytes[1]);
			Wire.write(bytes[0]);
		} else {
			Wire.write(bytes[0]);
			Wire.write(bytes[1]);
		}
	}
	Wire.endTransmission();
}

/**
 * Updates the display with decimal numbers
 */
void SAA1064::updateLED(long num)
{
	int Thousands, Hundreds, Tens, Base;
	Wire.beginTransmission(_address);
	Wire.write(1);
	num = num%10000; //kill every number bigger than 1000
	Thousands = (num/1000);
    Hundreds = ((num-(Thousands*1000))/100);
    Tens = ((num-((Thousands*1000)+(Hundreds*100)))/10);
    Base = (num-((Thousands*1000)+(Hundreds*100)+(Tens*10)));
	
	Thousands = numbers[Thousands];
	Hundreds  = numbers[Hundreds];
	Tens	  = numbers[Tens];
	Base	  = numbers[Base];
		switch (_dp) {
			case 1:
				Tens |= POINT;
				break;
			case 2:
				Hundreds |= POINT;
				break;
			case 3:
				Thousands |= POINT;
				break;
			default:
				break;
		}
	if (_command & MULTIPLEX) {
		if(_reverse_digit_order) {
			Wire.write(Thousands);
			Wire.write(Hundreds);
			Wire.write(Tens);
			Wire.write(Base);
		} else {
			Wire.write(Base);
			Wire.write(Tens);
			Wire.write(Hundreds);
			Wire.write(Thousands);
		}
	} else {
		if(_reverse_digit_order) {
			Wire.write(Tens);
			Wire.write(Base);
		} else {
			Wire.write(Base);
			Wire.write(Tens);
		}
	}
	Wire.endTransmission();
}

/**
 * Sends the command byte to the display processor
 */
void SAA1064::sendCommand()
{
	Wire.beginTransmission(_address);
	Wire.write(0);
	Wire.write(_command);
	Wire.endTransmission();
}