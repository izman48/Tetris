#ifndef _TETROMINOS_H
#define _TETROMINOS_H

#include <memory>
#include "Board.h"

using namespace std;

class Tetrominos {

    int size;
    int type;
    shared_ptr <Board> board;

    public:
        Tetrominos(int positionX, int positionY, shared_ptr<Board> board_);
        Tetrominos();
        void display();
        void displayAtPoint(float x, float y);
        void rotate();
        void rotateccw();
        void changeType();
        bool down();
        void left();
        void right();
        void setPosition(float x, float y);
        
        int posX;
        int posY;
        int rotation;
        bool currentShape[16];
        bool active;
        float colour[3];

};
#endif