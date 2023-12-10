# SnakeGame
Classic snake game played with Arduino buttons written in C language

You need to set up your arduino to be able to play this game.

The arduino uses atmega328P/PA.
You need to indicate the location of the library folder in the platform.ini file as the location of your lib folder in your computer.


- This is a simplified version of the famous snake game with Arduino and 4 digit display. The main purpose of the game is to bring the snake to the target point.
  - The game starts with pressing button 1. There is an interrupt for starting the game, when button 1 is pressed this interrupt occurs and it initializes the game environment by calling the randomlyPlaceTheSnake() function and starts the timer. randomlyPlaceTheSnake() function updates the currentDigit and currentSegment values by giving them random values to place on the display.
  - The snake is getting light up on display with the lightUpHorizontalSegments() and lightUpVerticalSegments() functions from display library. These functions only light up the "A, G and D" segments and "B and E" segments. The lightUpHorizontalSegments() function gets currentDigit, currentSegment and the snakeLength variables as parameters. currentDigit and currentSegment variables are getting their values from the arrays I defined in the display library.
  - The target point is getting displayed with the lightUpDpPoint() function from the display library. This function gets a random digit value as a parameter. The target point gets displayed in different digits randomly in each round. 
  - The direction of the snake is determined in the displayGame() function. This function checks for the button presses and updates the value of currentDirection variable. According to the current direction the corresponding move functions are called in the timer interrupt and the snake goes to the specific direction. The displayGame() function calls the display functions to light up the snake and the target point.
  - The timer is checking for 3 different things. It manages the snake's movement, updates the display, and checks the round timer. The timer interrupt is setting the snakeSpeed according to the in which level the user is. The speed gets 25% faster with every level increase. The timer is updating the value of roundTimer variable every second to check if the game takes more than 10 seconds. If the roundTimer variable becomes 0, in the displayGame() function stops the timer and the game ends. To be able to start again the user has to press button 1. Here I am updating all the values and initializing again by setting them to the beginning values of the game. For example, the level and gameRound becomes 1 again. 
  - There are 3 move functions; moveLeft(), moveRight() and moveUpDown(). moveLeft is basically decrementing the currentDigit variable and moveRight is incrementing the currentDigit variable. moveUpDown() function is called when the currentDirection is 2 meaning the middle button is pressed. It is initializing a verticalSegment variable and gives a random value of 1 or 0. Because the snake goes to a higher or lower horizontal segment randomly. This random value is lighting up the B or E segments on the led display. The new horizontal path is determined here, the snake goes to one of the horizontal paths passing along the verticalSegments. After pressing button 1 or button 3 the snake continues to left or right on the horizontal path. 
  - The updateConsecutiveMoves() function is checking if the snake has completely covered the same horizontal path twice. Then ends the game by stopping the timer and waits for the user to press button 1 to start the game again.
  - When the target is reached or when the game ends, arduino makes a sound.
  - When the buttons are pressed the corresponding led lights up briefly.
  - The score of the game is calculated in each round. A maximum of 15 points is possible at each level. The score depends on the number of button presses in each round. It is calculated as score = 3 * level * 5 - nrButtonPressed.
  - The game ends when the roundTimer becomes 0 and when the snake covers the same horizontal path twice. You can start the game again by pressing button 1.

ALPEREN DOGANCI