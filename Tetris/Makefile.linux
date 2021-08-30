GLEWPATH = /modules/cs324/glew-1.11.0/
INCDIRS= -I$(GLEWPATH)/include 
LIBDIRS= -L/usr/X11R6/lib -L$(GLEWPATH)/lib
LDLIBS = -lglut -lGL -lGLU -lX11 -lm -lGLEW

CPPFLAGS= $(INCDIRS) -O3 -std=c++11 -g
LDFLAGS= $(CPPFLAGS) $(LIBDIRS) -Wl,-rpath,$(GLEWPATH)/lib


CC = g++
FILES = Tetris.cpp Board.cpp Game.cpp Tetrominos.cpp Square.cpp
# LIBS = -lglut -lGL -lGLU -lX11 -lm -lGLEW -std=c++11

all:
	$(CC) $(FILES) $(LDLIBS) $(LDFLAGS) -o Tetris
clean:
	rm Tetris