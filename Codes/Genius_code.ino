#include <stdlib.h>
#include <time.h>

unsigned int size;
unsigned int *order;
bool play;

void setup()
{
  // Sets output pins
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  
  // Sets input pins
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);

  // Sets pin for powering buttons
  digitalWrite(13, HIGH);
  
  // Sets initial value of vars
  size = 3;
  play = false;
  srand(time(NULL));
  // play = false => game is outputting the correct order for the player
  // play = true => game is receiving inputs from the player
}

void loop()
{
  // Code to create a new button order outputting it
  if (play == false) {
    // Allocs memory according to the dificulty level (size)
    order = (unsigned int*)malloc(size * sizeof(unsigned int));
    for (int i = 0; i < size; i++) {
      // Chooses random number for each position
      order[i] = rand() % 4;
      // Shines the corresponding LED, and makes sound
      digitalWrite(order[i], HIGH);
      tone(12, 150 + 50*order[i]);
      delay(1000);
      digitalWrite(order[i], LOW);
      noTone(12);
      delay(200);
    }
    // Starts game
    play = true;
  }
  // Main game loop => receives inputs
  else {
    // Initial position on the order array = 0
    int pos = 0;
    // Loops game until player fails or completes the challenge
    while (pos < size && play == true) {
      // Loop for checking inputs on each of the 4 buttons
      for (int i = 0; i < 4; i++) {
        if (digitalRead(4 + i) == HIGH) {
          // If received input is correct, shine corresponding LED and jumps to next check
          if (i == order[pos]) {
            pos++;
            tone(12, 150 + i*50);
            digitalWrite(i, HIGH);
            delay(200);
            digitalWrite(i, LOW);
            noTone(12);
          }
          // If input is incorrect, shine all LEDs and restart game
          else {
            digitalWrite(0, HIGH);
            digitalWrite(1, HIGH); 
            digitalWrite(2, HIGH); 
            digitalWrite(3, HIGH);
            tone(12, 100);
            delay(1500);
            digitalWrite(0, LOW); 
            digitalWrite(1, LOW); 
            digitalWrite(2, LOW); 
            digitalWrite(3, LOW);
            noTone(12);
            // Redefine size and restarts game
            size = 3;
            play = false;
            break;
          }
        }
      }
    }
    // If the game was succesfully completed, increase size and restart
    if (pos >= size && play == true) {
      size++;
      play = false;
    }
    // Free allocated order when the game restarts
    free(order);
  }
  // Small delay
  delay(1000);
}