#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/freeglut.h>
#endif

#include "Tetrominos.h"
#include "Square.h"
#include "Board.h"
#include "Tetris.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <memory>
#include <string>

// Initialise Board
Board :: Board(int w_, int h_) {
    startH = h_;
    startW = w_;
    endW = startW + BOARD_WIDTH * SIZE;
    endH = startH - BOARD_HEIGHT * SIZE;
    points = 0;
    linesCleared = 0;
    level = 0;
}

// Display Board
void Board :: display() {
    drawBoard();
    drawBoardSquares();
}

// Draw the square inside of board
void Board :: drawBoardSquares() {

    // for each element of the array check if a square needs to be drawn and draw it
    glUseProgram(shader_program);
    for (int x  = 0; x < BOARD_WIDTH; x++) {
        for (int y = 0;  y < BOARD_HEIGHT; y++) {
            blocks[x][y].display();
        }
    }
}

// Draw the board
void Board :: drawBoard() {

    glColor3f(1.f, 1.f, 1.f);

    // Display Score and Level
    glRasterPos2i(startW + BOARD_WIDTH/2*SIZE - 1.5*SIZE, startH + 120);
    glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
    std::string text = "Score: " + std::to_string(points) + "\n" + "Level: " + std::to_string(level+1);
    const unsigned char* t = reinterpret_cast<const unsigned char *>(text.c_str());

    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, t);
    
    // draw a box of squares around the board coordinates
    drawBox(startW, startH, endW, endH);
}

// Draw a box around given parameters
void Board :: drawBox(float x, float y, float endX, float endY) {

    for (float i = x-SIZE; i < endX+SIZE ; i += SIZE){
        Square sq(i,y,.5f,.5f,.5f);
        sq.display();
    }
    for (float i = y; i >= endY-SIZE ; i -= SIZE){
        Square sq(endX,i,.5f,.5f,.5f);
        sq.display();
    }
    for (float i = x-SIZE; i < endX+SIZE ; i += SIZE){
        Square sq(i,endY-SIZE,.5f,.5f,.5f);
        sq.display();
    }
    for (float i = y; i >= endY-SIZE ; i -= SIZE){
        Square sq(x-SIZE,i,.5f,.5f,.5f);
        sq.display();
    }
}

// Store a tetromino in the board array
void Board :: storePiece(bool tetro_block[16], int posX, int posY, float colour[3]) {

	// ADD TO ARRAY

    // iterate through the bool of the tetromino square. When we get an active position, initialise a square at the position of the block 
    for (int i = 0; i < 16; i++) {
        int x = (i % 4 );
        int y = i / 4 ;
        
        if (tetro_block[i]) {
            int indexX = (posX - startW)/SIZE + x ;
            int indexY = (posY - endH)/SIZE - y;

            blocks[indexX][indexY] = Square(posX + x* SIZE, posY - y * SIZE, colour[0], colour[1], colour[2]);
        }
        
    }

	// CHECK IF LINES HAVE BEEN CLEARED

    // now that we stored the piece check if there are any lines
    // if there are, remove them and then move the array down a position
	// decrement j and check again to make the lines cleared cascading

    int count = 0;

    for (int j = 0; j < BOARD_HEIGHT; j++) {
        
		//check if the current line has been cleared
		bool line = true;
        for (int i = 0; i < BOARD_WIDTH; i++) {
            line = line && blocks[i][j].active;
        }

		// if it has been cleared, make the higher Squares take their place
        if (line) {
            count++;
            for (int y = j; y < BOARD_HEIGHT; y++) {
                for (int x = 0; x < BOARD_WIDTH; x++) {
                    if (y == BOARD_HEIGHT) {
                        blocks[x][y] = Square();
                    } else {
                        blocks[x][y] = blocks[x][y+1];
                        blocks[x][y].y -= SIZE;
                    }
                }
            }
            j--;
        }
        
            
    }

	// ADD TO SCORE AND ADJUST DIFFICULTY

    linesCleared += count;
    
	// every 10 levels increase difficulty
    if (linesCleared-10*level > 10) {
        level++;
    }

	// calculate score
    switch (count) {
        case 1: points += (1+level)*40; break;
        case 2: points += (1+level)*100; break;
        case 3: points += (1+level)*300; break;
        case 4: points += (1+level)*1200; break;

    }

    
}

// check if a piece has collisions with the board. Returns true if there is a collision
bool Board :: has_collision(bool tetro_blocks[16], int posX, int posY) {

	// Find the left, right and bottom block from the passed list of bools

    int left_block = 5;
    int right_block = -1;
    int bottom_block = -1;

	// calculate index and assign to variables if active
    for (int i = 0; i < 16; i++) {
        int x = (i % 4);
        int y = (floor((float)i / 4 ));

        if (tetro_blocks[i]) {
            if (x < left_block) {
                left_block = x;
            }
            if (x > right_block) {
                right_block = x;
            }
            if (y > bottom_block) {
                bottom_block = y;
            }
        }
    }

    // check if left or right block is hitting the border
    if (posX + left_block*SIZE < startW) {
        return true;
    }
    if (posX + right_block*SIZE >= endW) {
        return true;
    }
    
    // check if bottom block is hitting the bottom
    if (posY - bottom_block*SIZE < endH) {
        return true;
    }
    
	// check if any blocks are hitting another piece which is currently on the board
    for (int i = 0; i < 16; i++) {
        if (tetro_blocks[i]) {
            int x = (i % 4 ) ;
            int y = (floor((float)i / 4 ) ) ;
            int indexX = (posX - startW)/SIZE + x ;
            int indexY = (posY - endH)/SIZE - y;

            if (blocks[indexX][indexY].active) {
                return true;
            }
        } 
    }
    
	// if not return false
    return false;
}