#include "Chip8_Input.hpp"

extern const int KEY_NUM;

Chip8_Input::Chip8_Input() { clear_input(); }

Chip8_Input::~Chip8_Input() {}

void Chip8_Input::clear_input() {
  for (int i = 0; i < KEY_NUM; i++) {
    keypad[i] = false;
  }
}

bool Chip8_Input::is_key_pressed(int key) { return keypad[key]; }

void Chip8_Input::update_keys() {
  keypad[1] = sf::Keyboard::isKeyPressed(sf::Keyboard::Num1);
  keypad[2] = sf::Keyboard::isKeyPressed(sf::Keyboard::Num2);
  keypad[3] = sf::Keyboard::isKeyPressed(sf::Keyboard::Num3);
  keypad[4] = sf::Keyboard::isKeyPressed(sf::Keyboard::Q);
  keypad[5] = sf::Keyboard::isKeyPressed(sf::Keyboard::W);
  keypad[6] = sf::Keyboard::isKeyPressed(sf::Keyboard::E);
  keypad[7] = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
  keypad[8] = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
  keypad[9] = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
  keypad[10] = sf::Keyboard::isKeyPressed(sf::Keyboard::Z);
  keypad[0] = sf::Keyboard::isKeyPressed(sf::Keyboard::X);
  keypad[11] = sf::Keyboard::isKeyPressed(sf::Keyboard::C);
  keypad[12] = sf::Keyboard::isKeyPressed(sf::Keyboard::Num4);
  keypad[13] = sf::Keyboard::isKeyPressed(sf::Keyboard::R);
  keypad[14] = sf::Keyboard::isKeyPressed(sf::Keyboard::F);
  keypad[15] = sf::Keyboard::isKeyPressed(sf::Keyboard::V);
}