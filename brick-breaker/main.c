#include "main.h"

#include <stdio.h>
#include <stdlib.h>

#include "gba.h"
#include "play.h"
#include "background.h"
/* TODO: */
// Include any header files for title screen or exit
// screen images generated by nin10kit. Example for the provided garbage
// image:
// #include "images/garbage.h"

/* TODO: */
// Add any additional states you need for your app. You are not requried to use
// these specific provided states.
enum gba_state {
  START,
  WAITFORSTART,
  PLAY,
  WIN,
  LOSE,
  WAITRESTART,
  WINPAGE,
};

int main(void) {
  /* TODO: */
  // Manipulate REG_DISPCNT here to set Mode 3. //
  REG_DISPCNT = MODE3 | BG2_ENABLE;

  // Save current and previous state of button input.
  u32 previousButtons = BUTTONS;
  u32 currentButtons = BUTTONS;

  // Load initial application state
  enum gba_state state = START;

  while (1) {
    currentButtons = BUTTONS; // Load the current state of the buttons

    /* TODO: */
    // Manipulate the state machine below as needed //
    // NOTE: Call waitForVBlank() before you draw
    
    struct Ball bp;
    
    struct Paddle bar = {HEIGHT - 10, WIDTH / 2, 30, 5, 2};

    bp.row = HEIGHT / 2;
    bp.col = WIDTH / 2;
    bp.rd = 1;
    bp.cd = 1;
    bp.radius = 5;
    bp.color = GREEN;

    int nextState;
    char buffer[51];
    // int sc = 0;
    switch (state) {
      case START:
        waitForVBlank();
        drawFullScreenImageDMA(background);
        state = WAITFORSTART;
        break;
      case WAITFORSTART:
        currentButtons = BUTTONS;
        if (KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons) != 0) {
          state = PLAY;
          break;
        }
        previousButtons = currentButtons;
        waitForVBlank();
        sprintf(buffer, "Press START to play...");
        drawRectDMA(150, 5, 200, 10, BLACK);
        drawString(150, 5, buffer, WHITE);
        break;
      case PLAY:
        while (1) {
          currentButtons = BUTTONS;
          nextState = play(bp, bar);
          if (nextState == 0) {
            state = START;
            break;
          } else if (nextState == 1) {
            state = WIN;
            break;
          } else if (nextState == -1) {
            state = LOSE;
            break;
          }
        }
        break;
      case WIN:
        waitForVBlank();
        drawFullScreenImageDMA(background);
        state = WINPAGE;
        break;
      case LOSE:
        waitForVBlank();
        drawFullScreenImageDMA(background);
        state = WAITRESTART;
        break;
      case WINPAGE:
        while (1) {
          currentButtons = BUTTONS;
          if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons)) {
            state = START;
            break;
          }
          waitForVBlank();
          sprintf(buffer, "YOU WON!");
          drawString(100, 50, buffer, WHITE);
          drawString(120, 50, "Press Backspace to Return", WHITE);
        }
      case WAITRESTART:
        while (1) {
          currentButtons = BUTTONS;
          if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons)) {
            state = START;
            break;
          }
          waitForVBlank();
          drawString(100, 50, "YOU LOST :(", WHITE);
          drawString(120, 50, "Press Backspace to Return", WHITE);
        }
    }
    previousButtons = currentButtons; // Store the current state of the buttons
  }

  return 0;
}
