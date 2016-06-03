CC=g++
CFLAGS=-c
SOURCES := \
	src/main.cpp \
	src/Screen.cpp \
	src/Chip8.cpp

OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=Vigilant-chip8

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE) : $(OBJECTS)
	$(CC) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm src/*.o
	rm $(EXECUTABLE)
