#include <util/delay.h>
#include <avr/io.h>
#include <usart.h>

void enableBuzzer();

void playTone(float frequency, uint32_t duration);

void turnOffSound();