#ifndef SCREEN_H
#define SCREEN_H
#include <stdint.h>
#include <iostream>
#include <iomanip>

const int SCREEN_WIDTH = 64;
const int SCREEN_HEIGHT = 32;

class Screen {
  // The screen is a 64 * 32 pixel display with each pixel represented as a bit
  // Each row is 8 bytes long
  uint8_t screen_mem[((SCREEN_WIDTH/8) * SCREEN_HEIGHT)];

public:

  Screen();
  
  ~Screen();

  void draw_byte(uint8_t d_byte, uint8_t x, uint8_t y);

  void clear_screen();

  void show_screen();
};

#endif
