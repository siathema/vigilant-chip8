CC=g++
CFLAGS=-std=c++11 -g -c
LFLAGS=-g -std=c++11
SOURCES := \
	src/main.cpp \
	src/Screen.cpp \
	src/Chip8.cpp \
	src/Chip8_Input.cpp \
	src/debugger/Debugger.cpp

LIBS=-lsfml-graphics -lsfml-window -lsfml-system
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=Vigilant-chip8

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE) : $(OBJECTS)
	$(CC) $(LFLAGS) $(OBJECTS) -o $@ $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm src/*.o
	rm src/debugger/*.o
	rm $(EXECUTABLE)
