#ifndef _CP_PS2_JOYSTICK_
#define _CP_PS2_JOYSTICK_
#include <Arduino.h>

#define USE_JOYSTICK 1


// 0: unmove; 1: right; 2: bottom; 3: left; 4: top
#define MOVE_NONE 0
#define MOVE_UP 1
#define MOVE_RIGHT 2
#define MOVE_DOWN 3
#define MOVE_LEFT 4
#define BUTTON_DOWN 5


typedef struct _ps2_joystick {
  int x;
  int y;
  int k;
  int left;
  int right;
} ps2_joystick;

class CPPS2Joystick {
public:
  CPPS2Joystick();
  void init();
  ps2_joystick readAnalog();
  int readMove();
};

#endif
