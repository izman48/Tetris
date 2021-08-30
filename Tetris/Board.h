#ifndef _BOARD_H
#define _BOARD_H
#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20

#include "Square.h"

#include <memory>

using namespace std;

class Board {

    Square blocks[BOARD_WIDTH][BOARD_HEIGHT+2];

    public:
        Board(int w_, int h_);
        void display();
        void drawBoard();
        void drawBoardSquares();
        void drawBox(float x, float y, float xEnd, float yEnd);
        void storePiece(bool tetro_block[16], int cur_x, int cur_y, float colour[3]);
        bool has_collision(bool tetro_blocks[16], int cur_x, int cur_y);
        
        int startH;
        int startW;
        int endH;
        int endW;
        int linesCleared;
        int level;
        int points;

};
#endif

