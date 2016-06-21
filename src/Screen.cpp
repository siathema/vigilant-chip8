#include "Screen.hpp"

Screen::Screen(int w, int h) : window(sf::VideoMode(w, h), "Vigilent-Chip8") {
  init();
  clear_screen();
}

Screen::~Screen() { delete[] pixels; }

void Screen::init() {
  pixels = new sf::Uint8[SCREEN_WIDTH * SCREEN_HEIGHT * 4];
  image.create(SCREEN_WIDTH, SCREEN_HEIGHT);
}

bool Screen::draw_byte(uint8_t d_byte, uint8_t x, uint8_t y) {
  bool is_collision = false;
  int rollover_index = 0;

  for (int i = 0; i < 8; i++) {
    if (((d_byte << i) & 0x80) == 0x80) {
      if (x + i >= SCREEN_WIDTH) {
        if (screen_array[(SCREEN_WIDTH * y) + rollover_index]) {
          screen_array[(SCREEN_WIDTH * y) + rollover_index] = false;
          is_collision = true;
        } else {
          screen_array[(SCREEN_WIDTH * y) + rollover_index] = true;
          is_collision = false;
        }
        rollover_index++;
      } else if (screen_array[(SCREEN_WIDTH * y) + x + i]) {
        screen_array[(SCREEN_WIDTH * y) + x + i] = false;
        is_collision = true;
      } else {
        screen_array[(SCREEN_WIDTH * y) + x + i] = true;
        is_collision = false;
      }
    } else {
      if (i + x >= SCREEN_WIDTH) {
        rollover_index++;
      }
    }
  }

  return is_collision;
}

void Screen::render_screen() {
  for (int i = 0; i < (SCREEN_WIDTH * SCREEN_HEIGHT); i++) {
      for (int j = 0; j < 4; j++) {
        pixels[i * 4 + j] = screen_array[i] ? 0xFF : 0x00;
      }
  }

  image.update(pixels);
}

void Screen::clear_screen() {
  for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT * 4; i++) {
    pixels[i] = 0x00;
  }
  for (int i = 0; i < (SCREEN_WIDTH)*SCREEN_HEIGHT; i++) {
    screen_array[i] = false;
  }
}

void Screen::show_screen() {
  window.clear(sf::Color::Black);

  render_screen();

  sf::Sprite sprite;
  sprite.setTexture(image);
  sprite.setScale(sf::Vector2f(10.f, 10.f));

  window.draw(sprite);

  window.display();
}

sf::RenderWindow& Screen::get_window() { return window; }
