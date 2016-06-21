#ifndef CHIP8_INPUT_H
#define CHIP8_INPUT_H

#include <SFML/Window.hpp>

const int KEY_NUM = 16;

class Chip8_Input {
  bool keypad[KEY_NUM];

 public:
  Chip8_Input();

  ~Chip8_Input();

  void clear_input();

  bool is_key_pressed(int key);

  void update_keys();
};

#endif
