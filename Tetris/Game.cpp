#ifdef __APPLE__
#include <GLUT/freeglut.h>
#else
#include <GL/glew.h>
#include <GL/freeglut.h>
#endif

#include <cmath>
#include <iostream>
#include <vector>
#include <chrono>
#include <iostream>
#include <sys/time.h>
#include <ctime>


#include "Square.h"
#include "Tetrominos.h"
#include "Board.h"
#include "Game.h"
#include "Tetris.h"

using namespace std;

struct timeval time_now;

// Game constructor
Game :: Game(int width_, int height_)
{
    width = width_;
    height = height_;
    boardStartX = width/4;
    boardStartY = height* 3/4;
    blockSpawnX = (int)(boardStartX + SIZE*BOARD_WIDTH/2 - SIZE*2);
    blockSpawnY = boardStartY + SIZE;
    canHold = true;
    sPressed = false;
    gameOver = false;
    pause = false;
    moveSpeed = 500;
    board = std :: make_shared<Board>(boardStartX, boardStartY);
    activeTetromino = std :: make_shared<Tetrominos>(blockSpawnX, blockSpawnY, board);
    heldTetromino = std :: make_shared<Tetrominos>();
    nextTetromino = std :: make_shared<Tetrominos>(blockSpawnX, blockSpawnY, board);
    gettimeofday(&time_now, NULL);
    time_t start_time = (time_now.tv_sec * 1000) + (time_now.tv_usec / 1000);

}

// Resets Game board
void Game :: reset() {

    width = 1920;
    height = 1080;
    boardStartX = width/4;
    boardStartY = height* 3/4;
    blockSpawnX = (int)(boardStartX + SIZE*BOARD_WIDTH/2 - SIZE*2);
    blockSpawnY = boardStartY + SIZE;
    canHold = true;
    sPressed = false;
    gameOver = false;
    pause = false;
    moveSpeed = 500;
    board.reset(new Board(boardStartX, boardStartY));
    activeTetromino.reset(new Tetrominos((int)(blockSpawnX), blockSpawnY, board));
    heldTetromino.reset(new Tetrominos());
    nextTetromino.reset(new Tetrominos(blockSpawnX, blockSpawnY, board));
    gettimeofday(&time_now, NULL);
    time_t start_time = (time_now.tv_sec * 1000) + (time_now.tv_usec / 1000);
}


// displays all info for the game
void Game :: display()
{
	glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(0);

    glLoadIdentity();
    glPushMatrix();
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, width, 0, height);

    if (pause) {
        // Pause screen
        glColor3f(1.f, 1.f, 1.f);

        // Display Pause Text
        glRasterPos2i(board->startW + BOARD_WIDTH/2*SIZE - 1.5*SIZE, board->startH + 120);
        glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
        std::string text = string("PAUSED") + "\n" + "PRESS 'TAB' KEY TO RESUME";
        const unsigned char* t = reinterpret_cast<const unsigned char *>(text.c_str());

        glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, t);
        
    } else {
        if (gameOver) {
            // Gameover screen
            glColor3f(1.f, 1.f, 1.f);

            // Display Game Over Text
            glRasterPos2i(board->startW + BOARD_WIDTH/2*SIZE - 1.5*SIZE, board->startH + 120);
            glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
            std::string text = string("GAME OVER") + "\n"+ "SCORE: " + std :: to_string(board->points) + "\n"+ "LEVEL: " + std :: to_string(board->level + 1) +"\n" + "PRESS 'K' KEY TO RESET";
            const unsigned char* t = reinterpret_cast<const unsigned char *>(text.c_str());

            glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, t);

        } else {

            drawHeldPiece();
            drawNextPiece();

            glUseProgram(shader_program); // use shader for drawing board and pieces
            activeTetromino->display();
            board->display();

        }
    }
    
    glPopMatrix();
	glutSwapBuffers(); // swap the backbuffer with the front
}

// listen for keys pressed
void Game :: keyboard(unsigned char key, int, int) {

    // if game is not over then pause and unpause
    if ((int)key == 9 && !gameOver) {
        pause = !pause;
    }
    // if we haven't paused or we press k when game is over
    if (!pause || (gameOver && key == 'k')){

        // do commands for each corresponding
        switch (key)
        {   
            case 'e': activeTetromino->rotate(); break;

            case 'q': activeTetromino->rotateccw(); break;

            case 'x': exit(1); break;

            // case 't': activeTetromino->changeType();break; // used for debugging

            case 'a': activeTetromino->left(); break;

            case 'd': activeTetromino->right(); break;

            case 's': sPressed = true; break;

            case 'c': holdPiece(); break;

            case ' ': hardDrop(); break;

            case 'k': if (gameOver) reset(); break;

        }
    }

    // display changes
    display();
	glutPostRedisplay();
}

// listen for keys released
void Game :: keyboardRelease(unsigned char key, int a, int b) {
    if (key == 's') {
        sPressed = false;
    }
}

// Draw our Held Piece
void Game :: drawHeldPiece() {

    glUseProgram(0);

    // calculate position of the held box
    float heldBoxW = board->startW - SIZE * 10;
    float heldBoxH = board->startH - SIZE * 2;

    glColor3f(1.f, 1.f, 1.f);

    // Display Held Piece Text
    glRasterPos2i(heldBoxW , heldBoxH + SIZE*2);
    glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
    std::string text = "Held Piece";
    const unsigned char* t = reinterpret_cast<const unsigned char *>(text.c_str());
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, t);

    // Draw holding box
    glUseProgram(shader_program);
    board->drawBox(heldBoxW, heldBoxH, heldBoxW + 4*SIZE, heldBoxH - 4*SIZE);

    // If there is a held piece, draw it
    if (heldTetromino && heldTetromino->active) {
        heldTetromino->displayAtPoint(heldBoxW, heldBoxH - SIZE);
    }
}

// Draw our next piece
void Game :: drawNextPiece() {

    glUseProgram(0);

    // calculate positions of the next piece box
    float nextBoxW = board->endW + SIZE * 5;
    float nextBoxH = board->startH - SIZE * 2;

    glColor3f(1.f, 1.f, 1.f);

    // Display Next Piece Text
    glRasterPos2i(nextBoxW , nextBoxH + SIZE*2);
    glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
    std::string text = "Next Piece";
    const unsigned char* t = reinterpret_cast<const unsigned char *>(text.c_str());
    glutBitmapString(GLUT_BITMAP_TIMES_ROMAN_24, t);

    // Draw the next piece
    if (nextTetromino && nextTetromino->active) {
        glUseProgram(shader_program);
        nextTetromino->displayAtPoint(nextBoxW, nextBoxH - SIZE);
    }
}

// Instantly drop a piece
void Game :: hardDrop() {
    bool goDown = true;

    while (goDown) {
        
        // If piece can no longer go down place it and create a new piece
        if (!activeTetromino->down()) {
            activeTetromino = nextTetromino;
            nextTetromino.reset(new Tetrominos(blockSpawnX, blockSpawnY, board));

            // check if placing this piece has ended the game
            if (board->has_collision(activeTetromino->currentShape,activeTetromino->posX,activeTetromino->posY)) {
                gameOver = true;
            }

            canHold = true;
            goDown = false;
        }
    }
}

// Piece automatically moves down
void Game ::  moveDown() {

    gettimeofday(&time_now, nullptr);
    time_t current_time = (time_now.tv_sec * 1000) + (time_now.tv_usec / 1000);

    // calculate how fast piece must fall based on if we are soft dropping and level 
    int localSpeed;
    if (sPressed) {
        localSpeed = moveSpeed / 3;
    } else {
        localSpeed = moveSpeed;
    }
    localSpeed -= board->level * 10;

    // If it is time for piece to fall make it drop
    if (current_time - start_time > localSpeed) {

        start_time = current_time;  // reset time

        // If piece can no longer go down, place it
        if (!activeTetromino->down()) {
            activeTetromino = nextTetromino;
            nextTetromino.reset(new Tetrominos(blockSpawnX, blockSpawnY, board));
            if (board->has_collision(activeTetromino->currentShape,activeTetromino->posX,activeTetromino->posY)) {
                gameOver = true;
            }
            canHold = true;
        }

        // draw changes
        display();
        glutPostRedisplay();
    }
}

// We can hold a piece once every piece placed
void Game :: holdPiece() {

    // if this piece can be held
    if (canHold) {

        shared_ptr<Tetrominos> temp = activeTetromino;
        
        // if there is a held tetromino we replace active with held if not we create a new piece
        if (heldTetromino->active) {
            activeTetromino = heldTetromino;
            activeTetromino->setPosition((boardStartX + SIZE*BOARD_WIDTH/2 - SIZE*2), boardStartY);
        } else {
            activeTetromino = nextTetromino;
            nextTetromino.reset(new Tetrominos((int)(boardStartX + SIZE*BOARD_WIDTH/2 - SIZE*2), boardStartY, board)); 
        }
        heldTetromino = temp;
        canHold = false;
    }
}

