#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <led.h>
#include <button.h>
#include <display.h>
#include <usart.h>
#include <music.h>

#define MAX_SNAKE_LENGTH 3
#define NUM_DIGITS 4
#define NUM_HORIZONTAL_SEGMENTS 3
#define NUM_ROUNDS 3

#define MAX_CONSECUTIVE_MOVES 8 

int gameStarted = 0;

int snakeLength = 1;

int snakeSpeed= 100;
volatile uint16_t snakeMoveTime = 0;

int currentDirection = 0; // -1 for left, 1 for right, 2 for updown
int currentSegment = 0;
int currentDigit = 0;
int targetDigit;

int score;
int level = 1;
int gameRound;
int nrButtonPressed = 0;
bool targetReached = false;


bool checkTheMoves = false;


int timer_count = 0;
int roundTimer = 10;
const int MULTIPLE = 250;


// if the snake covered the horizontal path twice
int previousHorizontalPath = -1;
int consecutiveMoves = 0;


//FREQUENCIES OF THE NOTES
const float NOTES[] = {523.250, 698.460, 987.770, 1046.500, 783.990, 587.330, 659.250, 880.00};


void targetReachedSound(){
  for(int i = 0; i < 3; i++){
    playTone(NOTES[3], 50);
  }
}

void gameEndedSound(){
  for(int i = 0; i < 3; i++){
    playTone(NOTES[5], 100);
  }
}


void randomlyPlaceTheSnake() {

    int randomDigit = rand()  % NUM_DIGITS; // 0 1 2 3
    int randomSegment = rand()   % (NUM_HORIZONTAL_SEGMENTS - 1); // A G 

    targetDigit = rand() % NUM_DIGITS;
    printf("New target point is at digit %d\n", targetDigit);

    currentSegment = randomSegment;
    currentDigit = randomDigit;

    // Show the snake in a random horizontal place
    lightUpHorizontalSegments(currentDigit, currentSegment, snakeLength);

    printf("Initial location of the snake: currentDigit = %d, currentSegment = %d\n", currentDigit, currentSegment);
    
}


//To check if the snake covered the same horizontal path twice
void updateConsecutiveMoves(int newHorizontalPath) {
    if (newHorizontalPath == previousHorizontalPath) {
        consecutiveMoves++;
        if (consecutiveMoves >= MAX_CONSECUTIVE_MOVES - 1 ) {
            
            stopTimer();
            gameStarted = 0;
            gameEndedSound();
            
            printf("\nGame ended\n");
            printf("You covered the same horizontal path twice ! \n");
            printf("Your score: %d\n", score);
            writeNumber(score);

            snakeSpeed = 100;            
            snakeLength = 1;
            level = 1;
            gameRound = 1;
            score = 0;
                 
            consecutiveMoves = 0;
            printf("To start again press button 1\n");

        }
    } else {
        consecutiveMoves = 0;
    }
    previousHorizontalPath = newHorizontalPath;
}


void moveLeft() {
    
    if (currentDirection == -1) {

        currentDigit--;
        if (currentDigit < 0) {
            currentDigit = NUM_DIGITS - 1;
        }

      printf("Move Left: currentDigit = %d, currentSegment = %d, snakeLength = %d\n", currentDigit, currentSegment, snakeLength);    

      updateConsecutiveMoves(currentSegment);

    }
}

void moveRight() {
    
    if (currentDirection == 1) {

        currentDigit++;
        if (currentDigit >= NUM_DIGITS) {
            currentDigit = 0; 
        }

    printf("Move Right: currentDigit = %d, currentSegment = %d, snakeLength = %d\n", currentDigit, currentSegment, snakeLength);

    updateConsecutiveMoves(currentSegment);

    }
}
    

void moveUpDown() {
    if (currentDirection == 2) {

    int newSegment;

    // the snake will go to a different horizontal path
    consecutiveMoves = 0;
    
    // Move to a randomly selected vertical segment (B or E segment)
    int verticalSegment = currentDigit % 2;

    // light up segment B or E
    lightUpVerticalSegments(currentDigit, verticalSegment); 

     _delay_ms(100);
    // Switch between horizontal segments (A, G, D) based on the current vertical segment (B or E)
      if (verticalSegment == 0) { // B
          if (currentSegment == 0) {
              newSegment = 1; // A to G
        } else if (currentSegment == 1) {
              newSegment = 0; // G to A
          } else if (currentSegment == 2){
              newSegment = 1; // D to G
          }
      } else if(verticalSegment == 1){ // E
          if (currentSegment == 1) {
              newSegment = 2; // G to D
        } else if (currentSegment == 2) {
              newSegment = 1; // D to G
          } else if (currentSegment == 0){
              newSegment = 2; // A to D
          }
      }

    lightUpHorizontalSegments(currentDigit, newSegment, snakeLength);

    currentSegment = newSegment;

    printf("Move Up/Down: currentDigit = %d, currentSegment = %d\n", currentDigit, currentSegment);

    _delay_ms(100);

    }
}





ISR(PCINT1_vect) {

    if (bit_is_clear(PINC, PC1)) {

      if (!gameStarted) {
            
            lightUpAllLeds();
            _delay_ms(300);
            lightDownAllLeds();

            printf("The snake game is starting ....\n");
            
            gameStarted = 1;
            roundTimer = 10;

            randomlyPlaceTheSnake();

            checkTheMoves = false;
            startTimer();
            
        }
    }
             if(bit_is_clear(PINC, PC1)){
                moveLeft();
            } else if(bit_is_clear(PINC, PC2)){
                moveUpDown();
            }
}


ISR(TIMER2_COMPA_vect) {
    
    snakeMoveTime++;
    
    // to check if the game round took 10 seconds
    timer_count++;
    if (timer_count % MULTIPLE == 0 && timer_count != 0) {
        roundTimer--;
        printf(" %d\n", roundTimer);
    }

    // Increases snake speed 25% based on the level
    double currentSnakeSpeed = snakeSpeed * (1.0 - (level - 1) * 0.25);

    // Check if it's time to move the snake based on the current speed.
    if (snakeMoveTime >= currentSnakeSpeed) {
        snakeMoveTime = 0;
    
         if (currentDirection == -1) {
            moveLeft();
          } else if (currentDirection == 1) {
            moveRight();
          } else if (currentDirection == 2) {
            moveUpDown();
          }
    }

}


void initTimer(){
    TCCR2A |= _BV(WGM21); // CTC Mode
    TIMSK2 |= _BV(OCIE2A); // Enable Timer2 Compare A Match interrupt
    // Set OCR2A to the match value for 10 seconds (approx 10^7 cycles @ prescaler 64)
    OCR2A = 249; // Use the calculated match value for 10 seconds

    sei();    
}
void startTimer() {
    TCCR2B |= _BV(CS22) | _BV(CS21); // Set prescaler to 64 and start Timer, prescaler factor is now 1024 
}
void stopTimer() {
    TCCR2B &= ~_BV(CS22) & ~_BV(CS21); // Stop Timer by setting prescaler to 0
}


void updateTheGameStatus(){
    
    if (!targetReached && currentSegment == 2 && targetDigit == currentDigit) {
        targetReached = true;
        printf("TARGET REACHED !\n");
        targetReachedSound();
        
        // sets the timer to 10 seconds again
        roundTimer = 10;

        snakeLength++;
        if (snakeLength > MAX_SNAKE_LENGTH) {
            // Limit the snake length to the maximum allowed length
            snakeLength = 1;
        }
        printf("Length of the snake %d\n", snakeLength);

        randomlyPlaceTheSnake();
       
        gameStarted = 1;

        gameRound++;

        // Calculates and updates score
        int score = 3 * level * 5 - nrButtonPressed;
        printf("Number of button pressing %d\n", nrButtonPressed);
        nrButtonPressed = 0;

        // increments the level and speed after round 3
        if (gameRound > NUM_ROUNDS) {
            level++;
            gameRound = 1; // Resets round
        }

        printf("\n Level: %d", level);
        printf("\n Round: %d", gameRound);
        printf("\n Score: %d\n", score);

        printf("\nNew round is starting ... \n");

        targetReached = false;

   }
}

void updateDisplaySnakeAndFood(){
    lightUpHorizontalSegments(currentDigit, currentSegment, snakeLength);
    lightUpDpPoint(targetDigit);
}



void displayGame(){
// movement logic
if(!checkTheMoves){

   // Check if button 1 (moveLeft) is pressed
    if (bit_is_clear(PINC, PC1)) {
        nrButtonPressed++;
        lightUpLed(0);
        _delay_ms(100);
        lightDownLed(0);
        currentDirection = -1;
            }

    // Check if button 2 (moveUpDown) is pressed
    else if (bit_is_clear(PINC, PC2)) {
            nrButtonPressed++;
            lightUpLed(1);
            _delay_ms(100);
            lightDownLed(1);
            currentDirection = 2;
            }
            

    // Check if button 3 (moveRight) is pressed
    else if (bit_is_clear(PINC, PC3)) {
            nrButtonPressed++;
            lightUpLed(2);
            _delay_ms(100);
            lightDownLed(2);
            currentDirection = 1;
            } 

    
    updateDisplaySnakeAndFood();
 
    // if 10 seconds passed
    if(roundTimer == 0){
            roundTimer = 10;
            stopTimer();
            printString("Game ended!\n");
            printString("Time's up! The round took more than 10 seconds\n");
            printf("Your score: %d\n", score);

            writeNumber(score);

            snakeSpeed= 100;
            snakeLength = 1;
            level = 1;
            gameRound = 1;
            score = 0;
            gameStarted = 0;
            printString("To start the game again press button 1\n");

    }   

    // check if target reached
    if(currentSegment == 2 && currentDigit == targetDigit){
        updateTheGameStatus();    
        }

    }
}


void activateInterrupt(){
    PCICR |= _BV(PCIE1); // Port C interrupt activate 
    PCMSK1 |=  _BV(PC1) | _BV(PC2) | _BV(PC3);  // Indicate which pins of that port indicate button1

    sei(); // activate the interrupt globally
}

int main() {
    
    initUSART();
    initDisplay();
    enableAllLeds();
    enableButton(1);
    enableButton(2);
    enableButton(3);

    
    enableBuzzer();
    turnOffSound();
    
    initTimer();

    printf("\nTHE SNAKE GAME\n");
    printf("You have 10 seconds to complete a round.\n");
    printf("If the snake completely covers the same horizontal path twice the game ends.\n");
    printf("To start the game press button 1\n");

    activateInterrupt(); //interrupt for button 1 to start the game

       
while (1) {
    
        displayGame();

    }

    return 0;
}