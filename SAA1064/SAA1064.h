/*
 *  SAA1064.h
 *  
 *
 *  Created by Christopher Parish on 19/01/2010.
 *  Copyright 2010 Chris Parish. All rights reserved.
 *
 */
#include <Arduino.h>
#include <inttypes.h>
#include <string.h>

/**
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * Constants
 */

// LED number lookup table
const int numbers[16] = {0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71};

// Brightness values
const byte BRIGHT_HI  =0x40;
const byte BRIGHT_MED =0x20;
const byte BRIGHT_LOW =0x10;
// Test value
const byte TEST_ALL   =0x08;
// Multiplex value
const byte MULTIPLEX  =0x07;
// Decimal point value
const byte POINT      =0x80;


/**
 * SAA1064 Class
 * Communicates with the SAA1064 LED microcontroller via I2C
 */
class SAA1064
{
public:
	SAA1064(int addr);
	void clear();
	void test();
	void multiplexOn();
	void multiplexOff();
	void digit_order(bool reversed);
	void setBrightness(char *brightness);
	void setDP(int point);
	void send(int num);
	void send(long num);
	void send(float num);
	void sendHex(uint16_t num);
private:
	void updateLED(long num);
	void sendCommand();
	uint8_t _address;
	uint8_t _command;
	uint8_t _digits;
	uint8_t _dp;
	bool _reverse_digit_order;
};