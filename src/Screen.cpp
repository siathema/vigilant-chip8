#include "Screen.hpp"

Screen::Screen() {
  clear_screen();
}

Screen::~Screen() {

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

void Screen::clear_screen() {

  for(int i=0; i < ((SCREEN_WIDTH/8)*SCREEN_HEIGHT); i++) {

    screen_mem[i] = 0x0000;

  }
  
}

void Screen::show_screen() {

  for(int i=0; i < (SCREEN_HEIGHT); i++) {
    
    for(int j=0; j < (SCREEN_WIDTH/8); j++) {

      std::cout << screen_mem[(i*SCREEN_WIDTH/8)+j];

    }

  }

}
