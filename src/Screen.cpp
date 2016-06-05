#include "Screen.hpp"

Screen::Screen(int w, int h): window(sf::VideoMode(w,h),"Vigilent-Chip8") {

  init();
  clear_screen();

}

Screen::~Screen() {
  delete[] pixels;
}

void Screen::init() {
  pixels = new sf::Uint8[SCREEN_WIDTH * SCREEN_HEIGHT * 4];
  image.create(SCREEN_WIDTH, SCREEN_HEIGHT);
}

bool Screen::draw_byte(uint8_t d_byte, uint8_t x, uint8_t y) {

  bool is_collision = false;
  
  for(int i=0; i<8; i++) {

    if(((d_byte<<i)&0x80) == 0x80) {
      if(x+i > SCREEN_WIDTH) {
	if(screen_array[(SCREEN_WIDTH*y)+i]) {
	  
	  screen_array[(SCREEN_WIDTH*y)+i] = false;
	  is_collision = true;
	  
	} else {

	  screen_array[(SCREEN_WIDTH*y)+i] = true;
	  is_collision = false;

	}
      } else if( screen_array[(SCREEN_WIDTH*y)+x+i]) {
	
	screen_array[(SCREEN_WIDTH*y)+x+i] = false;
	is_collision = true;

      } else {

	screen_array[(SCREEN_WIDTH*y)+x+i] = true;
	is_collision = false;
	
      }
    }
  }

  return is_collision;
  
}


void Screen::render_screen() {

  for(int i=0; i<(SCREEN_WIDTH*SCREEN_HEIGHT); i++) {
    if(screen_array[i]) {
      for(int j=0; j<4; j++) {
	pixels[i*4 + j] = 0xFF;
      }
    }
  }
  
  image.update(pixels);
  
}

void Screen::clear_screen() {

    for(int i=0; i < SCREEN_WIDTH*SCREEN_HEIGHT*4; i++) {

    pixels[i] = 0x00;

    }
  for(int i=0; i< (SCREEN_WIDTH)*SCREEN_HEIGHT; i++) {

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

sf::RenderWindow& Screen::get_window() {

  return window;
}
