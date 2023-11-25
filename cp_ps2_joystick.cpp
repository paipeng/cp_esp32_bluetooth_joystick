#include "cp_ps2_joystick.h"


#define VRX 2
#define VRY 0
#define BUTTON 4


#define BUTTON_LEFT 25
#define BUTTON_RIGHT 26
#define BUTTON_RESET 27

#define USE_JOYSTICK 1
#define DEBUG 1

CPPS2Joystick::CPPS2Joystick() {
  // init joystick
}

void CPPS2Joystick::init() {
  pinMode(BUTTON, INPUT);

  //pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);
  pinMode(BUTTON_RESET, INPUT_PULLUP);
}

ps2_joystick CPPS2Joystick::readAnalog() {
  ps2_joystick joystick;
#if USE_JOYSTICK
  // x,y: 0-4095 (right - left; center 2877)
  // k: 1 unpressed; 0: pressed
  joystick.x = analogRead(VRX);
  joystick.y = analogRead(VRY);
  //joystick.k = digitalRead(BUTTON);
  joystick.k = 1 - digitalRead(BUTTON_RESET);
#else
  joystick.left = digitalRead(BUTTON_LEFT);
  joystick.right = digitalRead(BUTTON_RIGHT);
  joystick.k = digitalRead(BUTTON_RESET);
#endif
#if DEBUG
  Serial.print("Joystick: ");
  Serial.printf("x: %4d y: %4d button: %d left: %d right: %d", joystick.x, joystick.y, joystick.k, joystick.left, joystick.right);
  Serial.println("");
#endif
  return joystick;
}

int CPPS2Joystick::readMove() {
  ps2_joystick joystick = readAnalog();

#if USE_JOYSTICK
  if (joystick.k == 1) {
    return BUTTON_DOWN;
  }
  if (joystick.x > 3000) {
    return MOVE_LEFT;
  } else if (joystick.x < 500) {
    return MOVE_RIGHT;
  } else if (joystick.y > 4000) {
    return MOVE_UP;
  } else if (joystick.y < 500) {
    return MOVE_DOWN;
  } else {
    return MOVE_NONE;
  }
#else
  if (joystick.k == 0) {
    return BUTTON_DOWN;
  }

  if (joystick.left == 0) {
    return MOVE_LEFT;
  } else if (joystick.right == 0) {
    return MOVE_RIGHT;
  } else {
    return MOVE_NONE;
  }
#endif
}