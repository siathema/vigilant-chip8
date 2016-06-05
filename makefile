CC=g++
CFLAGS=-c
SOURCES := \
	src/main.cpp \
	src/Screen.cpp \
	src/Chip8.cpp \
	src/Chip8_Input.cpp

LIBS=-lsfml-graphics -lsfml-window -lsfml-system
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=Vigilant-chip8

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE) : $(OBJECTS)
	$(CC) -g $(OBJECTS) -o $@ $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm src/*.o
	rm $(EXECUTABLE)
