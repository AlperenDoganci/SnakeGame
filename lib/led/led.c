#include <util/delay.h>
#include <avr/io.h>


#define NUMBER_OF_LEDS 4

void enableLed ( int lednumber ) //C has no classes; functions can be included directly in the .c file.
{
    if ( lednumber < 0 || lednumber > NUMBER_OF_LEDS-1 ) return;
    DDRB |= ( 1 << ( PB2 + lednumber ));    
}

void lightUpLed ( int lednumber )    //Note: enabled LEDs light up immediately ( 0 = on )
{
    if ( lednumber < 0 || lednumber > NUMBER_OF_LEDS-1 ) return;
    PORTB &= ~( 1 << ( PB2 + lednumber )); 
}

void lightDownLed ( int lednumber )
{
    if ( lednumber < 0 || lednumber > 3 ) return;
    PORTB |= ( 1 << ( PB2 + lednumber )); 
}


void enableMultipleLeds(uint8_t leds)
{  for (int i = 0; i < NUMBER_OF_LEDS; i++) {
   if (leds & (1 << i)){
     enableLed(i);  
     lightDownLed(i);
   } else {
     lightUpLed(i);
  }
}
}

void enableAllLeds(){
    for (int i = 0; i < NUMBER_OF_LEDS; i++)
    {
        enableLed(i);
    }
    
}

void lightUpMultipleLeds(uint8_t leds)
{  for (int i = 0; i < NUMBER_OF_LEDS; i++) {
   if (leds & (1 << i)){
     lightUpLed(i);
   } else {
     lightDownLed(i);
  }
}
}

void lightUpAllLeds(){
    for (int i = 0; i < NUMBER_OF_LEDS; i++)
    {
        lightUpLed(i);
    }
}


void lightDownMultipleLeds(uint8_t leds)
{  for (int i = 0; i < NUMBER_OF_LEDS; i++) {
   if (leds & (1 << i)){
     lightDownLed(i);
   } else {
     lightUpLed(i);
  }
}
}

void lightDownAllLeds(){
        for (int i = 0; i < NUMBER_OF_LEDS; i++)
    {
        lightDownLed(i);
    }
}

void lightToggleOneLed(int led){
    if(led < 0 || led > NUMBER_OF_LEDS - 1) return;
    while(1){
        lightUpLed(led);
        _delay_ms(1000);
        lightDownLed(led);
        _delay_ms(1000);
    }
}

void dimLed (int lednumber, int percentage, int duration){
    if(lednumber < 0 || lednumber > NUMBER_OF_LEDS - 1) return;

    for (int i = 0; i< duration; i++){
        lightUpLed(i);
        _delay_ms(percentage);
        lightDownLed(i);
        _delay_ms(100 / percentage);
    }
}

void  fadeInLed (int led, int duration) {
    if(led < 0 || led > NUMBER_OF_LEDS - 1) return;
    for (int i = 0; i <= 100; i++)
    {
        dimLed(led,i,20);
    }
    
}

void fadeOutLed(int led, int duration){
    if(led < 0 || led > NUMBER_OF_LEDS - 1) return;
    for (int i = 100; i > 0; i--)
    {
        dimLed(led,i,20);
    }
    
}