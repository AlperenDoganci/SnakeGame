#include <util/delay.h>
#include <avr/io.h>
#include <usart.h>

#define NR_OF_BUTTONS 3

void enableButton( int button ){
    if(button < 1 || button > NR_OF_BUTTONS ) return;
    
    switch (button){
    case 1:
        DDRC &= ~(1 << PC1);
        break;
    case 2:
        DDRC &= ~(1 << PC2);
    case 3:
        DDRC &= ~(1 << PC3);
        break;
        default:
        break;
    }
     
}
int buttonPushed( int button ){
    if(button < 1 || button > NR_OF_BUTTONS ) return;
    
    switch (button){
    case 1:
        if ((PINC & (1 << PC1)) == 0)
        {
            return 1;
        }
        break;
    case 2:
        if ((PINC & (1 << PC2)) == 0)
        {
            return 1;
        }
        break;
    case 3:
        if ((PINC & (1 << PC3)) == 0)
        {
            return 1;
        }
        break;           
        default:
        break;
    }
}



int buttonReleased( int button ){
    if(button < 1 || button > NR_OF_BUTTONS ) return;
        
    switch (button){
    case 1:
        if ((PINC & (1 << PC1)) == 1)
        {
            return 1;
        }
        break;
    case 2:
        if ((PINC & (1 << PC2)) == 1)
        {
            return 1;
        }
        break;
    case 3:
        if ((PINC & (1 << PC3)) == 1)
        {
            return 1;
        }
        break;           
        default:
        break;
    }
}