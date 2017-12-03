#pragma once

#include <mathernogl/MathernoGL.h>

#define MOUSE_LEFT = GLFW_MOUSE_BUTTON_1
#define MOUSE_RIGHT = GLFW_MOUSE_BUTTON_2
#define MOUSE_MIDDLE = GLFW_MOUSE_BUTTON_3

#define KEY_SPACE = GLFW_KEY_SPACE
#define KEY_ESC = GLFW_KEY_ESCAPE
#define KEY_ENTER = GLFW_KEY_ENTER
#define KEY_TAB = GLFW_KEY_TAB
#define KEY_BACKSPACE = GLFW_KEY_BACKSPACE
#define KEY_LSHIFT = GLFW_KEY_LEFT_SHIFT
#define KEY_RSHIFT = GLFW_KEY_RIGHT_SHIFT
#define KEY_LCTRL = GLFW_KEY_LEFT_CONTROL
#define KEY_RCTRL = GLFW_KEY_RIGHT_CONTROL
#define KEY_LALT = GLFW_KEY_LEFT_ALT
#define KEY_RALT = GLFW_KEY_RIGHT_ALT
#define KEY_UP = GLFW_KEY_UP
#define KEY_LEFT = GLFW_KEY_LEFT
#define KEY_DOWN = GLFW_KEY_DOWN
#define KEY_RIGHT = GLFW_KEY_RIGHT

#define KEY_F1 = GLFW_KEY_F1
#define KEY_F2 = GLFW_KEY_F2
#define KEY_F3 = GLFW_KEY_F3
#define KEY_F4 = GLFW_KEY_F4
#define KEY_F5 = GLFW_KEY_F5
#define KEY_F6 = GLFW_KEY_F6
#define KEY_F7 = GLFW_KEY_F7
#define KEY_F8 = GLFW_KEY_F8
#define KEY_F9 = GLFW_KEY_F9
#define KEY_F10 = GLFW_KEY_F10
#define KEY_F11 = GLFW_KEY_F11
#define KEY_F12 = GLFW_KEY_F12


/*
*   If the key code belongs to a character, then this will return that character, otherwise returns 0 ('\0')
*     Returns uppercase version of letters
*/
static char getCharFromKeyCode(uint code)
  {
  switch (code)
    {
    case GLFW_KEY_A:  return 'A';
    case GLFW_KEY_B:  return 'B';
    case GLFW_KEY_C:  return 'C';
    case GLFW_KEY_D:  return 'D';
    case GLFW_KEY_E:  return 'E';
    case GLFW_KEY_F:  return 'F';
    case GLFW_KEY_G:  return 'G';
    case GLFW_KEY_H:  return 'H';
    case GLFW_KEY_I:  return 'I';
    case GLFW_KEY_J:  return 'J';
    case GLFW_KEY_K:  return 'K';
    case GLFW_KEY_L:  return 'L';
    case GLFW_KEY_M:  return 'M';
    case GLFW_KEY_N:  return 'N';
    case GLFW_KEY_O:  return 'O';
    case GLFW_KEY_P:  return 'P';
    case GLFW_KEY_Q:  return 'Q';
    case GLFW_KEY_R:  return 'R';
    case GLFW_KEY_S:  return 'S';
    case GLFW_KEY_T:  return 'T';
    case GLFW_KEY_U:  return 'U';
    case GLFW_KEY_V:  return 'V';
    case GLFW_KEY_W:  return 'W';
    case GLFW_KEY_X:  return 'X';
    case GLFW_KEY_Y:  return 'Y';
    case GLFW_KEY_Z:  return 'Z';
    case GLFW_KEY_1:  return '1';
    case GLFW_KEY_2:  return '2';
    case GLFW_KEY_3:  return '3';
    case GLFW_KEY_4:  return '4';
    case GLFW_KEY_5:  return '5';
    case GLFW_KEY_6:  return '6';
    case GLFW_KEY_7:  return '7';
    case GLFW_KEY_8:  return '8';
    case GLFW_KEY_9:  return '9';
    case GLFW_KEY_0:  return '0';
    case GLFW_KEY_MINUS:  return '-';
    case GLFW_KEY_EQUAL:  return '=';
    }
  }

