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

void Screen::draw_byte(uint8_t d_byte, uint8_t x, uint8_t y) {
  if(x%8 == 0 || x%8 == 8) { // byte aligns with screen_mem
    
    screen_mem[(y*SCREEN_WIDTH/8) + x/8] = d_byte;
    
  } else { // byte is between screen_mem bytes

    int mem_index = x/8;
    int bit_index = x%8;
    uint8_t hi_mask = d_byte>>bit_index;
    uint8_t lo_mask = d_byte<< (8 - bit_index);
    screen_mem[(y*SCREEN_WIDTH/8) + mem_index] ^= hi_mask;
    screen_mem[(y*SCREEN_WIDTH/8) + (mem_index + 1)] ^= lo_mask;

  }
}

void Screen::render_screen() {

  for(int i=0; i < ((SCREEN_WIDTH/8)*SCREEN_HEIGHT); i++) {

    if(screen_mem[i] == 0x00) {
      // screen is empty here, move along
      continue;
    } else {

      uint8_t byte = screen_mem[i];
      for(int j=0; j<8; j++) {

	if((byte<<j & 0x80)== 0x80) {
	  for(int w=0; w<4; w++) {

	    pixels[(i*8)+(j*4)+w] = 255;

	  }
	}
	
      }

    }

  }

  image.update(pixels);
  
}

void Screen::clear_screen() {

  for(int i=0; i < ((SCREEN_WIDTH/8)*SCREEN_HEIGHT); i++) {

    screen_mem[i] = 0x0000;
    for(int j=0; j<8*4; j++) {

      pixels[i+j] = 0;
      
    }

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
