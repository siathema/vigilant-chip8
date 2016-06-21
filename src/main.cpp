#include <iostream>
#include <fstream>
#include <iomanip>
#include <string.h>
#include <string.h>
#include <stdint.h>
#include "Chip8.hpp"
#include "debugger/Debugger.hpp"

int main(int argc, char** argv)
{

    bool debug = false;

    if (argc > 3 && argc < 2) {
        std::cout << "usage: Vigilant-chip8 -options <Path to file>" << std::endl;
        return 0;
    }
    if (argc == 3) {
        if (strcmp(argv[1], "--debugger") != 0) {
            std::cout << "Unrecognized option!\n";
            return -1;
        }

        std::cout << "Entering debugging mode.\n";
        debug = true;
    }

    char* m_pathToFile = argc == 2 ? argv[1] : argv[2];
    std::streampos m_file_size;
    char* m_buffer;

    std::ifstream m_fileStream;
    m_fileStream.open(m_pathToFile, std::ios::in | std::ios::binary | std::ios::ate);
    if (m_fileStream.is_open()) {
        m_file_size = m_fileStream.tellg();
        m_buffer = new char[m_file_size];
        m_fileStream.seekg(0, std::ios::beg);
        m_fileStream.read(m_buffer, m_file_size);
        m_fileStream.close();
    }
    else {
        std::cout << "Cannot open file: " << std::endl;
        return -1;
    }

    Chip8 chip8(m_buffer, m_file_size);
    if (debug) {

        Debugger debugger;
        debugger.run(chip8);
    }
    else {

        while (1) {

            chip8.update(false);
        }
    }
    delete[] m_buffer;

    return 0;
}
