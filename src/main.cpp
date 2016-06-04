#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdint.h>
//#include "Chip8.hpp"
#include "Screen.hpp"

int main(int argc, char ** argv) {
  
  if(argc != 2){
    std::cout << "usage: Vigilant-chip8 <Path to file>" << std::endl;
    return 0;
  }

  char * m_pathToFile = argv[1];

  std::streampos m_file_size;
  char * m_buffer;

  std::ifstream m_fileStream;
  m_fileStream.open(m_pathToFile, std::ios::in|std::ios::binary|std::ios::ate);
  if (m_fileStream.is_open()) {
    m_file_size = m_fileStream.tellg();
    m_buffer = new char [m_file_size];
    m_fileStream.seekg(0, std::ios::beg);
    m_fileStream.read(m_buffer, m_file_size);
    m_fileStream.close();
  } else {
    std::cout << "Cannot open file: " << std::endl;
    return -1;
  }


  uint8_t hex_char[] = {
    0xF0, 0x90, 0x90, 0x90, 0xf0
  };
  
  Screen screen(640, 320);

  for(int i=0; i<5; i++) {

    screen.draw_byte(hex_char[i], 10, i);

  }
  
  while(true) {
    
    screen.show_screen();
  }
  
  delete m_buffer;
  
  return 0;
}
