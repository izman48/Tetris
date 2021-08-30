#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <iostream>
#include <math.h>

#include "Square.h"
#include "Tetris.h"

// Initialise a square
Square :: Square(float x_, float y_, float r_, float g_, float b_) {
    red = r_;
    green = g_;
    blue = b_;
    x = x_;
    y = y_;
    active = true;
}

// Empty constructor for a square
Square :: Square() {
    active = false;
    red = 0;
    green = 0;
    blue = 0;
    x = 0;
    y = 0;
}

// display a square
void Square :: display() {   

    if (!active) return;

    // Update position at the shader with position of the square
    glUniform2f(glGetUniformLocation(shader_program, "position"), (float)x, (float)y);

    glColor3f(red, green, blue);
    glBegin(GL_POLYGON); // filled polygon
        float xEnd = x + SIZE;
        float yEnd = y + SIZE;
        glVertex2f(x,y);
        glVertex2f(xEnd,y);
        glVertex2f(xEnd,yEnd);
        glVertex2f(x,yEnd);
    glEnd();
}

