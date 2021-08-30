#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include "Tetrominos.h"
#include "Board.h"
#include "Game.h"
#include "PieceInfo.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

using namespace std;

// Constructer for Tetromino
Tetrominos :: Tetrominos(int positionX, int positionY, shared_ptr<Board> board_) {
    rotation = 0;
    active = true;
    srand (time(NULL));
    board = board_;
    type = (rand() % 7);
    posX = positionX;
    posY = positionY;
    size = 16;
    for (int i = 0; i < size; i++) {
        currentShape[i] = shapes[type][rotation][i];
    }
    for (int i = 0; i < 3; i++) {
        colour[i] = colours[type][i];
    }
}

// Construct for an inactive piece
Tetrominos :: Tetrominos() {
    active= false;
}

// Display Tetromino 
void Tetrominos :: display() {

    if (!active) return;

    // draw each square where there is an active bool
    int width = sqrt(size);

    for (int i = 0; i < size; i++) {
        int px = (i % width ) * SIZE;
        int py = (floor((float)i / width ) ) * SIZE;
        if (currentShape[i]) {
            Square sq(posX + px, posY - py, colour[0], colour[1], colour[2]);
            sq.display();
        }
    }
}

// Used to display held piece and next piece at point x,y
void Tetrominos :: displayAtPoint(float x, float y) {

    // set the current rotation to 0 and update current shape
    rotation = 0;
    int width = sqrt(size);
    for (int i = 0; i < 16; i++) {
        currentShape[i] = shapes[type][rotation][i];
    }

    // draw each square where there is an active bool
    for (int i = 0; i < size; i++) {
        int px = (i % width ) * SIZE;
        int py = (floor((float)i / width ) ) * SIZE;
        if (currentShape[i]) {
            Square sq(x + px, y - py, colour[0], colour[1], colour[2]);
            sq.display();
        }

    }
}

// Used for testing. Switches the type of eachpiece
void Tetrominos :: changeType() {

    int newType = (type + 1) % 7;
    int newRotation = 0;
    if (!board->has_collision(shapes[newType][newRotation], posX, posY)) {
        type = newType;
        rotation = newRotation;
        for (int i = 0; i < 16; i++) {
            currentShape[i] = shapes[type][rotation][i];
        }
        for (int i = 0; i < 3; i++) {
            colour[i] = colours[type][i];
        }
    }
    
}

// Sets the position of the piece
void Tetrominos :: setPosition(float x, float y) {
    posX = x;
    posY = y;
}

// Moves down if there is no collision. If not, it tries to store the piece. Returns true if it successfully moved down
bool Tetrominos :: down() {
    if (!board->has_collision(currentShape, posX, posY - SIZE)){
        posY -= SIZE;
        return true;
    } else {
        board->storePiece(currentShape, posX, posY, colour);
    }
    return false;
}

// moves left if there is no collision
void Tetrominos :: left() {
    if (!board->has_collision(currentShape, posX - SIZE, posY)){
        posX -= SIZE;
    }
}

// moves right if there is no collision
void Tetrominos :: right() {
    if (!board->has_collision(currentShape, posX + SIZE, posY)){
        posX += SIZE;
    }
    
}

// rotates piece clockwise
void Tetrominos :: rotate() {

    rotation = (rotation + 1) % 4;

    // if the piece has no collisions, it is successful. If not it attempts wall kicks which are used in the original games.
    // wall kicks are when pieces are moved slightly to fit a rotation.
    // There are different wall kicks for each rotation depending on clockwise/counter-clockwise, piece type and original position
    if (!board->has_collision(shapes[type][rotation], posX, posY)) {
        for (int i = 0; i < 16; i++) {
            currentShape[i] = shapes[type][rotation][i];
        }
    } else {
        if (type > 1) {
            if (rotation == 1) {
                if (!board->has_collision(shapes[type][rotation], posX - SIZE, posY)) {
                    posX -= SIZE;
                    for (int i = 0; i < 16; i++) {
                        currentShape[i] = shapes[type][rotation][i];
                    }
                } else {
                    if (!board->has_collision(shapes[type][rotation], posX - SIZE, posY + SIZE)) {
                        posX -= SIZE;
                        posY += SIZE;
                        for (int i = 0; i < 16; i++) {
                            currentShape[i] = shapes[type][rotation][i];
                        }
                    } else {
                        if (!board->has_collision(shapes[type][rotation], posX, posY - 2*SIZE)) {
                            posY -= 2*SIZE;
                            for (int i = 0; i < 16; i++) {
                                currentShape[i] = shapes[type][rotation][i];
                            }
                        } else {
                            if (!board->has_collision(shapes[type][rotation], posX - SIZE, posY - 2*SIZE)) {
                                posX -= SIZE;
                                posY -= 2*SIZE;
                                for (int i = 0; i < 16; i++) {
                                    currentShape[i] = shapes[type][rotation][i];
                                }
                            } else {
                                rotation = (rotation - 1) % 4;
                                return;
                            }
                        }
                    }
                }
                
            }
            if (rotation == 2) {
                if (!board->has_collision(shapes[type][rotation], posX + SIZE, posY)) {
                    posX += SIZE;
                    for (int i = 0; i < 16; i++) {
                        currentShape[i] = shapes[type][rotation][i];
                    }
                } else {
                    if (!board->has_collision(shapes[type][rotation], posX + SIZE, posY - SIZE)) {
                        posX += SIZE;
                        posY -= SIZE;
                        for (int i = 0; i < 16; i++) {
                            currentShape[i] = shapes[type][rotation][i];
                        }
                    } else {
                        if (!board->has_collision(shapes[type][rotation], posX, posY + 2*SIZE)) {
                            posY += 2*SIZE;
                            for (int i = 0; i < 16; i++) {
                                currentShape[i] = shapes[type][rotation][i];
                            }
                        } else {
                            if (!board->has_collision(shapes[type][rotation], posX + SIZE, posY + 2*SIZE)) {
                                posX += SIZE;
                                posY += 2*SIZE;
                                for (int i = 0; i < 16; i++) {
                                    currentShape[i] = shapes[type][rotation][i];
                                }
                            } else {
                                rotation = (rotation - 1) % 4;
                                return;
                            }
                        }
                    }
                }

            }
            if (rotation == 3) {
                if (!board->has_collision(shapes[type][rotation], posX + SIZE, posY)) {
                    posX += SIZE;
                    for (int i = 0; i < 16; i++) {
                        currentShape[i] = shapes[type][rotation][i];
                    }
                } else {
                    if (!board->has_collision(shapes[type][rotation], posX + SIZE, posY + SIZE)) {
                        posX += SIZE;
                        posY += SIZE;
                        for (int i = 0; i < 16; i++) {
                            currentShape[i] = shapes[type][rotation][i];
                        }
                    } else {
                        if (!board->has_collision(shapes[type][rotation], posX, posY - 2*SIZE)) {
                            posY -= 2*SIZE;
                            for (int i = 0; i < 16; i++) {
                                currentShape[i] = shapes[type][rotation][i];
                            }
                        } else {
                            if (!board->has_collision(shapes[type][rotation], posX + SIZE, posY - 2*SIZE)) {
                                posX += SIZE;
                                posY -= 2*SIZE;
                                for (int i = 0; i < 16; i++) {
                                    currentShape[i] = shapes[type][rotation][i];
                                }
                            } else {
                                rotation = (rotation - 1) % 4;
                                return;
                            }
                        }
                    }
                }
            }
            if (rotation == 0) {
                if (!board->has_collision(shapes[type][rotation], posX - SIZE, posY)) {
                    posX -= SIZE;
                    for (int i = 0; i < 16; i++) {
                        currentShape[i] = shapes[type][rotation][i];
                    }
                } else {
                    if (!board->has_collision(shapes[type][rotation], posX - SIZE, posY - SIZE)) {
                        posX -= SIZE;
                        posY -= SIZE;
                        for (int i = 0; i < 16; i++) {
                            currentShape[i] = shapes[type][rotation][i];
                        }
                    } else {
                        if (!board->has_collision(shapes[type][rotation], posX, posY + 2*SIZE)) {
                            posY += 2*SIZE;
                            for (int i = 0; i < 16; i++) {
                                currentShape[i] = shapes[type][rotation][i];
                            }
                        } else {
                            if (!board->has_collision(shapes[type][rotation], posX - SIZE, posY + 2*SIZE)) {
                                posX -= SIZE;
                                posY += 2*SIZE;
                                for (int i = 0; i < 16; i++) {
                                    currentShape[i] = shapes[type][rotation][i];
                                }
                            } else {
                                rotation = (rotation - 1) % 4;
                                return;
                            }
                        }
                    }
                }
            }
        } else {
            if (rotation == 1) {
                if (!board->has_collision(shapes[type][rotation], posX - 2*SIZE, posY)) {
                    posX -= 2*SIZE;
                    for (int i = 0; i < 16; i++) {
                        currentShape[i] = shapes[type][rotation][i];
                    }
                } else {
                    if (!board->has_collision(shapes[type][rotation], posX + SIZE, posY)) {
                        posX += SIZE;
                        // posY += SIZE;
                        for (int i = 0; i < 16; i++) {
                            currentShape[i] = shapes[type][rotation][i];
                        }
                    } else {
                        if (!board->has_collision(shapes[type][rotation], posX - 2*SIZE, posY - 1*SIZE)) {
                            posX -= 2*SIZE;
                            posY -= 1*SIZE;
                            for (int i = 0; i < 16; i++) {
                                currentShape[i] = shapes[type][rotation][i];
                            }
                        } else {
                            if (!board->has_collision(shapes[type][rotation], posX + SIZE, posY + 2*SIZE)) {
                                posX += SIZE;
                                posY += 2*SIZE;
                                for (int i = 0; i < 16; i++) {
                                    currentShape[i] = shapes[type][rotation][i];
                                }
                            } else {
                                rotation = (rotation - 1) % 4;
                                return;
                            }
                        }
                    }
                }
                
            }
            if (rotation == 2) {
                if (!board->has_collision(shapes[type][rotation], posX - SIZE, posY)) {
                    posX -= SIZE;
                    for (int i = 0; i < 16; i++) {
                        currentShape[i] = shapes[type][rotation][i];
                    }
                } else {
                    if (!board->has_collision(shapes[type][rotation], posX + 2*SIZE, posY)) {
                        posX += 2*SIZE;
                        // posY -= SIZE;
                        for (int i = 0; i < 16; i++) {
                            currentShape[i] = shapes[type][rotation][i];
                        }
                    } else {
                        if (!board->has_collision(shapes[type][rotation], posX - SIZE, posY + 2*SIZE)) {
                            posX -= SIZE;
                            posY += 2*SIZE;
                            for (int i = 0; i < 16; i++) {
                                currentShape[i] = shapes[type][rotation][i];
                            }
                        } else {
                            if (!board->has_collision(shapes[type][rotation], posX + 2*SIZE, posY - SIZE)) {
                                posX += 2*SIZE;
                                posY -= SIZE;
                                for (int i = 0; i < 16; i++) {
                                    currentShape[i] = shapes[type][rotation][i];
                                }
                            } else {
                                rotation = (rotation - 1) % 4;
                                return;
                            }
                        }
                    }
                }

            }
            if (rotation == 3) {
                if (!board->has_collision(shapes[type][rotation], posX + 2*SIZE, posY)) {
                    posX += 2*SIZE;
                    for (int i = 0; i < 16; i++) {
                        currentShape[i] = shapes[type][rotation][i];
                    }
                } else {
                    if (!board->has_collision(shapes[type][rotation], posX - SIZE, posY)) {
                        posX -= SIZE;
                        // posY += SIZE;
                        for (int i = 0; i < 16; i++) {
                            currentShape[i] = shapes[type][rotation][i];
                        }
                    } else {
                        if (!board->has_collision(shapes[type][rotation], posX + 2*SIZE, posY + SIZE)) {
                            posX += 2*SIZE;
                            posY += SIZE;
                            for (int i = 0; i < 16; i++) {
                                currentShape[i] = shapes[type][rotation][i];
                            }
                        } else {
                            if (!board->has_collision(shapes[type][rotation], posX - SIZE, posY - 2*SIZE)) {
                                posX -= SIZE;
                                posY -= 2*SIZE;
                                for (int i = 0; i < 16; i++) {
                                    currentShape[i] = shapes[type][rotation][i];
                                }
                            } else {
                                rotation = (rotation - 1) % 4;
                                return;
                            }
                        }
                    }
                }
            }
            if (rotation == 0) {
                if (!board->has_collision(shapes[type][rotation], posX + SIZE, posY)) {
                    posX += SIZE;
                    for (int i = 0; i < 16; i++) {
                        currentShape[i] = shapes[type][rotation][i];
                    }
                } else {
                    if (!board->has_collision(shapes[type][rotation], posX - 2*SIZE, posY)) {
                        posX -= 2*SIZE;
                        // posY -= SIZE;
                        for (int i = 0; i < 16; i++) {
                            currentShape[i] = shapes[type][rotation][i];
                        }
                    } else {
                        if (!board->has_collision(shapes[type][rotation], posX + SIZE, posY - 2*SIZE)) {
                            posX += SIZE;
                            posY -= 2*SIZE;
                            for (int i = 0; i < 16; i++) {
                                currentShape[i] = shapes[type][rotation][i];
                            }
                        } else {
                            if (!board->has_collision(shapes[type][rotation], posX - 2*SIZE, posY + SIZE)) {
                                posX -= 2*SIZE;
                                posY += SIZE;
                                for (int i = 0; i < 16; i++) {
                                    currentShape[i] = shapes[type][rotation][i];
                                }
                            } else {
                                rotation = (rotation - 1) % 4;
                                return;
                            }
                        }
                    }
                }
            }
        }
        
    }
}

// rotates piece counter-clockwise
void Tetrominos :: rotateccw() {
    
    rotation -= 1;
    if (rotation == -1) rotation = 3;

    // if the piece has no collisions, it is successful. If not it attempts wall kicks which are used in the original games.
    // wall kicks are when pieces are moved slightly to fit a rotation.
    // There are different wall kicks for each rotation depending on clockwise/counter-clockwise, piece type and original position
    if (!board->has_collision(shapes[type][rotation], posX, posY)) {
        for (int i = 0; i < 16; i++) {
            currentShape[i] = shapes[type][rotation][i];
        }
    } else {
        if (type > 1) {
            if (rotation == 1) {
                if (!board->has_collision(shapes[type][rotation], posX + SIZE, posY)) {
                    posX += SIZE;
                    for (int i = 0; i < 16; i++) {
                        currentShape[i] = shapes[type][rotation][i];
                    }
                } else {
                    if (!board->has_collision(shapes[type][rotation], posX + SIZE, posY - SIZE)) {
                        posX += SIZE;
                        posY -= SIZE;
                        for (int i = 0; i < 16; i++) {
                            currentShape[i] = shapes[type][rotation][i];
                        }
                    } else {
                        if (!board->has_collision(shapes[type][rotation], posX, posY + 2*SIZE)) {
                            posY += 2*SIZE;
                            for (int i = 0; i < 16; i++) {
                                currentShape[i] = shapes[type][rotation][i];
                            }
                        } else {
                            if (!board->has_collision(shapes[type][rotation], posX + SIZE, posY + 2*SIZE)) {
                                posX += SIZE;
                                posY += 2*SIZE;
                                for (int i = 0; i < 16; i++) {
                                    currentShape[i] = shapes[type][rotation][i];
                                }
                            } else {
                                if (++rotation == 4) rotation = 0;
                                return;
                            }
                        }
                    }
                }
                
            }
            if (rotation == 2) {
                if (!board->has_collision(shapes[type][rotation], posX - SIZE, posY)) {
                    posX -= SIZE;
                    for (int i = 0; i < 16; i++) {
                        currentShape[i] = shapes[type][rotation][i];
                    }
                } else {
                    if (!board->has_collision(shapes[type][rotation], posX - SIZE, posY + SIZE)) {
                        posX -= SIZE;
                        posY += SIZE;
                        for (int i = 0; i < 16; i++) {
                            currentShape[i] = shapes[type][rotation][i];
                        }
                    } else {
                        if (!board->has_collision(shapes[type][rotation], posX, posY - 2*SIZE)) {
                            posY -= 2*SIZE;
                            for (int i = 0; i < 16; i++) {
                                currentShape[i] = shapes[type][rotation][i];
                            }
                        } else {
                            if (!board->has_collision(shapes[type][rotation], posX - SIZE, posY - 2*SIZE)) {
                                posX -= SIZE;
                                posY -= 2*SIZE;
                                for (int i = 0; i < 16; i++) {
                                    currentShape[i] = shapes[type][rotation][i];
                                }
                            } else {
                                if (++rotation == 4) rotation = 0;
                                return;
                            }
                        }
                    }
                }

            }
            if (rotation == 3) {
                if (!board->has_collision(shapes[type][rotation], posX - SIZE, posY)) {
                    posX -= SIZE;
                    for (int i = 0; i < 16; i++) {
                        currentShape[i] = shapes[type][rotation][i];
                    }
                } else {
                    if (!board->has_collision(shapes[type][rotation], posX - SIZE, posY - SIZE)) {
                        posX -= SIZE;
                        posY -= SIZE;
                        for (int i = 0; i < 16; i++) {
                            currentShape[i] = shapes[type][rotation][i];
                        }
                    } else {
                        if (!board->has_collision(shapes[type][rotation], posX, posY + 2*SIZE)) {
                            posY += 2*SIZE;
                            for (int i = 0; i < 16; i++) {
                                currentShape[i] = shapes[type][rotation][i];
                            }
                        } else {
                            if (!board->has_collision(shapes[type][rotation], posX - SIZE, posY + 2*SIZE)) {
                                posX -= SIZE;
                                posY += 2*SIZE;
                                for (int i = 0; i < 16; i++) {
                                    currentShape[i] = shapes[type][rotation][i];
                                }
                            } else {
                                if (++rotation == 4) rotation = 0;
                                return;
                            }
                        }
                    }
                }
            }
            if (rotation == 0) {
                if (!board->has_collision(shapes[type][rotation], posX + SIZE, posY)) {
                    posX += SIZE;
                    for (int i = 0; i < 16; i++) {
                        currentShape[i] = shapes[type][rotation][i];
                    }
                } else {
                    if (!board->has_collision(shapes[type][rotation], posX + SIZE, posY + SIZE)) {
                        posX += SIZE;
                        posY += SIZE;
                        for (int i = 0; i < 16; i++) {
                            currentShape[i] = shapes[type][rotation][i];
                        }
                    } else {
                        if (!board->has_collision(shapes[type][rotation], posX, posY - 2*SIZE)) {
                            posY -= 2*SIZE;
                            for (int i = 0; i < 16; i++) {
                                currentShape[i] = shapes[type][rotation][i];
                            }
                        } else {
                            if (!board->has_collision(shapes[type][rotation], posX + SIZE, posY - 2*SIZE)) {
                                posX += SIZE;
                                posY -= 2*SIZE;
                                for (int i = 0; i < 16; i++) {
                                    currentShape[i] = shapes[type][rotation][i];
                                }
                            } else {
                                if (++rotation == 4) rotation = 0;
                                return;
                            }
                        }
                    }
                }
            }
        } else {
            if (rotation == 1) {
                if (!board->has_collision(shapes[type][rotation], posX + 2*SIZE, posY)) {
                    posX += 2*SIZE;
                    for (int i = 0; i < 16; i++) {
                        currentShape[i] = shapes[type][rotation][i];
                    }
                } else {
                    if (!board->has_collision(shapes[type][rotation], posX - SIZE, posY)) {
                        posX -= SIZE;
                        // posY += SIZE;
                        for (int i = 0; i < 16; i++) {
                            currentShape[i] = shapes[type][rotation][i];
                        }
                    } else {
                        if (!board->has_collision(shapes[type][rotation], posX + 2*SIZE, posY + 1*SIZE)) {
                            posX += 2*SIZE;
                            posY += 1*SIZE;
                            for (int i = 0; i < 16; i++) {
                                currentShape[i] = shapes[type][rotation][i];
                            }
                        } else {
                            if (!board->has_collision(shapes[type][rotation], posX - SIZE, posY - 2*SIZE)) {
                                posX -= SIZE;
                                posY -= 2*SIZE;
                                for (int i = 0; i < 16; i++) {
                                    currentShape[i] = shapes[type][rotation][i];
                                }
                            } else {
                                if (++rotation == 4) rotation = 0;
                                return;
                            }
                        }
                    }
                }
                
            }
            if (rotation == 2) {
                if (!board->has_collision(shapes[type][rotation], posX + SIZE, posY)) {
                    posX += SIZE;
                    for (int i = 0; i < 16; i++) {
                        currentShape[i] = shapes[type][rotation][i];
                    }
                } else {
                    if (!board->has_collision(shapes[type][rotation], posX - 2*SIZE, posY)) {
                        posX -= 2*SIZE;
                        // posY -= SIZE;
                        for (int i = 0; i < 16; i++) {
                            currentShape[i] = shapes[type][rotation][i];
                        }
                    } else {
                        if (!board->has_collision(shapes[type][rotation], posX + SIZE, posY - 2*SIZE)) {
                            posX += SIZE;
                            posY -= 2*SIZE;
                            for (int i = 0; i < 16; i++) {
                                currentShape[i] = shapes[type][rotation][i];
                            }
                        } else {
                            if (!board->has_collision(shapes[type][rotation], posX - 2*SIZE, posY + SIZE)) {
                                posX -= 2*SIZE;
                                posY += SIZE;
                                for (int i = 0; i < 16; i++) {
                                    currentShape[i] = shapes[type][rotation][i];
                                }
                            } else {
                                if (++rotation == 4) rotation = 0;
                                return;
                            }
                        }
                    }
                }

            }
            if (rotation == 3) {
                if (!board->has_collision(shapes[type][rotation], posX - 2*SIZE, posY)) {
                    posX -= 2*SIZE;
                    for (int i = 0; i < 16; i++) {
                        currentShape[i] = shapes[type][rotation][i];
                    }
                } else {
                    if (!board->has_collision(shapes[type][rotation], posX + SIZE, posY)) {
                        posX += SIZE;
                        // posY += SIZE;
                        for (int i = 0; i < 16; i++) {
                            currentShape[i] = shapes[type][rotation][i];
                        }
                    } else {
                        if (!board->has_collision(shapes[type][rotation], posX - 2*SIZE, posY - SIZE)) {
                            posX -= 2*SIZE;
                            posY -= SIZE;
                            for (int i = 0; i < 16; i++) {
                                currentShape[i] = shapes[type][rotation][i];
                            }
                        } else {
                            if (!board->has_collision(shapes[type][rotation], posX + SIZE, posY + 2*SIZE)) {
                                posX += SIZE;
                                posY += 2*SIZE;
                                for (int i = 0; i < 16; i++) {
                                    currentShape[i] = shapes[type][rotation][i];
                                }
                            } else {
                                if (++rotation == 4) rotation = 0;
                                return;
                            }
                        }
                    }
                }
            }
            if (rotation == 0) {
                if (!board->has_collision(shapes[type][rotation], posX - SIZE, posY)) {
                    posX -= SIZE;
                    for (int i = 0; i < 16; i++) {
                        currentShape[i] = shapes[type][rotation][i];
                    }
                } else {
                    if (!board->has_collision(shapes[type][rotation], posX + 2*SIZE, posY)) {
                        posX += 2*SIZE;
                        // posY -= SIZE;
                        for (int i = 0; i < 16; i++) {
                            currentShape[i] = shapes[type][rotation][i];
                        }
                    } else {
                        if (!board->has_collision(shapes[type][rotation], posX - SIZE, posY + 2*SIZE)) {
                            posX -+ SIZE;
                            posY += 2*SIZE;
                            for (int i = 0; i < 16; i++) {
                                currentShape[i] = shapes[type][rotation][i];
                            }
                        } else {
                            if (!board->has_collision(shapes[type][rotation], posX + 2*SIZE, posY - SIZE)) {
                                posX += 2*SIZE;
                                posY -= SIZE;
                                for (int i = 0; i < 16; i++) {
                                    currentShape[i] = shapes[type][rotation][i];
                                }
                            } else {
                                if (++rotation == 4) rotation = 0;
                                return;
                            }
                        }
                    }
                }
            }
        }
        
    }
}


