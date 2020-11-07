// Harmony of light and sound lab 
// Evan Von Oehsen eavb2018@mymail.pomona.edu
// with digital level starter code by David_Harris@hmc.edu and Josh Brake 11 October 2020

#include <stdint.h>
#include "ppm_ev.h"

/////////////////////////////////////////////////////////////////////
// SPI Functions
/////////////////////////////////////////////////////////////////////

int LED_PIN_5 = 19;
int LED_PIN_6 = 18;
int LED_PIN_7 = 16;
int LED_PIN_8 = 17;
int LED_PIN_16 = 0;
int LED_PIN_15 = 1;
int LED_PIN_14 = 9;
int LED_PIN_13 = 13;

int KEY_PRESS_INPUT = 21;
int KEY_CHANGE_INPUT = 23;

void keyPress(int selectedKey);
void keyRelease(int selectedKey);
void pitchBend(int selectedKey, int y);

#define NOTE_C4 262
#define NOTE_D4 294
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_G4 392
#define NOTE_A4 440
#define NOTE_B4 500
#define NOTE_C5 522

int ledWrite_x(int x, int prev);
int ledWrite_y(int y, int prev);

void spiWrite(uint8_t address, uint8_t value)
{
	uint8_t hi, lo;
	digitalWrite(2, 0); // pulse chip select
	hi = spiSendReceive(address);
	lo = spiSendReceive(value);
	digitalWrite(2, 1); // release chip select
						// discard returned values on a write transaction
}

uint8_t spiRead(uint8_t address)
{
	uint8_t hi, lo;
	digitalWrite(2, 0);					   // pulse chip select
	hi = spiSendReceive(address | 1 << 7); // set msb for reads
	lo = spiSendReceive(0x00);			   // send dummy payload
	digitalWrite(2, 1);					   // release chip select
	return lo;
}

/////////////////////////////////////////////////////////////////////
// Main Loop
/////////////////////////////////////////////////////////////////////

int main(void)
{
	uint8_t debug;
	int16_t x, y;

	//setup clocks and hardware
	spiInit(15, 0, 0);	// Initialize SPI pins and clocks
	digitalWrite(2, 1); // Manually control LIS3DH Chip Select
	pinMode(2, OUTPUT);
	debug = spiRead(0x0F);

	if (debug)
		digitalWrite(2, 1);
	spiWrite(0x20, 0x77); // highest conversion rate, all axis on
	spiWrite(0x23, 0x88); // block update, and high resolution

	int pins[8] = {LED_PIN_5, LED_PIN_6, LED_PIN_7, LED_PIN_8, LED_PIN_16, LED_PIN_15, LED_PIN_14, LED_PIN_13};
	int i;

	pinMode(KEY_CHANGE_INPUT, INPUT);
	pinMode(KEY_PRESS_INPUT, INPUT);

	for (i = 0; i <= 14; i++)
	{
		pinMode(pins[i], OUTPUT);
		digitalWrite(pins[i], 0);
	}
	digitalWrite(pins[0], 1);

	pwmInit();
	int selectedKey = 0;
	while (1)
	{
		if (digitalRead(KEY_PRESS_INPUT) == 1)
		{
			keyPress(selectedKey);
			while (digitalRead(KEY_PRESS_INPUT) == 1)
			{
				x = spiRead(0x28) | (spiRead(0x29) << 8);
				y = spiRead(0x2A) | (spiRead(0x2B) << 8);
				pitchBend(selectedKey, y);
			}
			keyRelease(selectedKey);
		}
		else if (digitalRead(KEY_CHANGE_INPUT) == 1)
		{
			while (digitalRead(KEY_CHANGE_INPUT) == 1)
				;
			digitalWrite(pins[selectedKey], 0);
			if (selectedKey <= 6)
				selectedKey++;
			else
				selectedKey = 0;
			digitalWrite(pins[selectedKey], 1);
		}
	}
}

void keyPress(int selectedKey)
{
	int notes[8] = {NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5};
	int pins[8] = {LED_PIN_5, LED_PIN_6, LED_PIN_7, LED_PIN_8, LED_PIN_16, LED_PIN_15, LED_PIN_14, LED_PIN_13};

	if (selectedKey == 0)
	{
		digitalWrite(pins[selectedKey + 1], 1);
	}
	else if (selectedKey == 7)
	{
		digitalWrite(pins[selectedKey - 1], 1);
	}
	else
	{
		digitalWrite(pins[selectedKey - 1], 1);
		digitalWrite(pins[selectedKey + 1], 1);
	}
	pwm(notes[selectedKey], 0.5);
}

void keyRelease(int selectedKey)
{
	int pins[8] = {LED_PIN_5, LED_PIN_6, LED_PIN_7, LED_PIN_8, LED_PIN_16, LED_PIN_15, LED_PIN_14, LED_PIN_13};

	if (selectedKey == 0)
	{
		digitalWrite(pins[selectedKey + 1], 0);
	}
	else if (selectedKey == 7)
	{
		digitalWrite(pins[selectedKey - 1], 0);
	}
	else
	{
		digitalWrite(pins[selectedKey - 1], 0);
		digitalWrite(pins[selectedKey + 1], 0);
	}
	pwm(0, 0.5);
}

void pitchBend(int selectedKey, int y)
{
	y += 1800; //offset for the tilt of my accelerometer's pins
	int notes[8] = {NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_C5};
	int pitch = notes[selectedKey] - (y / 800);
	pwm(pitch, 0.5);
}
