#ifndef SCREEN_H
#define SCREEN_H
#include <stdint.h>
#include <iostream>
#include <iomanip>
#include <SFML/Graphics.hpp>

const int SCREEN_WIDTH = 64;

const int SCREEN_HEIGHT = 32;

class Screen {
  // The screen is a 64 * 32 pixel display with each pixel represented as a bit
  // Each row is 8 bytes long
  bool screen_array[SCREEN_WIDTH*SCREEN_HEIGHT];

  sf::Uint8* pixels;
  
  sf::RenderWindow window;

  sf::Texture image;
  
  
public:

  Screen(int w, int h);
  
  ~Screen();

  void init();
  
  bool draw_byte(uint8_t d_byte, uint8_t x, uint8_t y);

  void clear_screen();

  sf::RenderWindow& get_window();
  
  void render_screen();
  
  void show_screen();

  
};

#endif
