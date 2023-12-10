#include <util/delay.h>
#include <avr/io.h>
#include <math.h>

void enableLed(int ledNumber);

void lightUpLed(int ledNumber);

void lightDownLed(int ledNumber);

void enableMultipleLeds(uint8_t leds);

void enableAllLeds();

void lightUpMultipleLeds(uint8_t leds);

void lightUpAllLeds(); 

void lightDownMultipleLeds(uint8_t leds);

void lightDownAllLeds(); 

void lightToggleOneLed(int led) ;