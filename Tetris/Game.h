#ifndef _GAME_H
#define _GAME_H

#include <memory>
#include <vector>

#include "Board.h"
#include "Square.h"
#include "Tetrominos.h"

using namespace std;

class Game {
    int boardStartX;
    int boardStartY;
    int blockSpawnX;
    int blockSpawnY;
    int moveSpeed;
    bool sPressed;

    shared_ptr <Board> board;
    shared_ptr <Tetrominos> activeTetromino;
    shared_ptr <Tetrominos> heldTetromino;
    shared_ptr <Tetrominos> nextTetromino;
    time_t start_time;

    public:
        Game(int width_, int height_);
        void reset();
        void display();
        void drawHeldPiece();
        void drawNextPiece();
        void holdPiece();
        void keyboard(unsigned char key, int, int);
        void keyboardRelease(unsigned char key, int, int);
        void moveDown();
        void hardDrop();

        int width;
        int height;
        bool canHold;
        bool gameOver;
        bool pause;
        
        
};
#endif

