#ifdef __APPLE__
#include <GLUT/freeglut.h>
#else
#include <GL/glew.h>
#include <GL/freeglut.h>
#endif
#include "Game.h"
#include "Tetris.h"
#include "Shader_utility.h"

#include <iostream>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

Game game(1920, 1080);  //Initialise game with a 1920 x 1080 aspect ratio

GLuint shader_program;
GLuint vertex_shader;
GLuint fragment_shader;

// display the game
void display() {
    game.display();
}

// listen for keys pressed
void keyboard(unsigned char key, int a, int b) {
    game.keyboard(key,a,b);
}

// listen for keys released
void keyboardRelease(unsigned char key, int a, int b) {
    game.keyboardRelease(key,a,b);
}

// Idle function which runs when nothing is happening
void idle() {
    if (!game.gameOver && !game.pause)
        game.moveDown();

}

// resizes window size in the shader
void windowResizeFunc(GLint width, GLint height) {
    glViewport(0,0, width, height);

    float a[4];
    glGetFloatv(GL_VIEWPORT, a);

    glUniform2f(glGetUniformLocation(shader_program, "windowSize"), a[2], a[3]);

}

// Initialise our shaders
void initShader() {

    // read the shader source files
	char* vertex_source = read_shader_source("vertex_shader.vert");
	char* frag_source = read_shader_source("fragment_shader.frag");

    glewInit();
    shader_program = glCreateProgram();

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, (const char**)&vertex_source, NULL);


    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, (const char**)&frag_source, NULL);



    glCompileShader(vertex_shader);

    // check if it has compile ok
    int compiled = 0;
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &compiled);
    if (compiled==0)
    {
        // failed to compile vertex shader
        fprintf(stderr, "Failed to compile vertex shader\n");
        print_shader_info_log(vertex_shader);

        exit(1);
    }
    
    glCompileShader(fragment_shader);

    // check if it has compile ok
    compiled = 0;
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &compiled);
    if (compiled==0)
    {
        // failed to compile vertex shader
        fprintf(stderr, "Failed to compile fragment shader\n");
        print_shader_info_log(fragment_shader);

        exit(1);
    }

    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);

    glLinkProgram(shader_program);
    glUseProgram(shader_program);
    
    // Initialise our values in our shader program
    glUniform1f(glGetUniformLocation(shader_program, "blockSize"), SIZE);
    glUniform2f(glGetUniformLocation(shader_program, "orthoWindowSize"), game.width, game.height);

}

// main function
int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
    

	glutInitWindowSize(game.width, game.height); // window size
	glutInitWindowPosition(0, 0); // where on screen

	glutCreateWindow("Tetris");

    glutKeyboardFunc(keyboard); 

    glutKeyboardUpFunc(keyboardRelease);

	glutDisplayFunc(display); // set display callback

    glutReshapeFunc(windowResizeFunc);

    glutIdleFunc(idle);

    // set the GL display mode: double buffering and RGBA colour mode
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA); // flags bitwise OR'd together

    initShader();

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // clear colour -- black!

	glutMainLoop(); // go into the main loop and wait for window events...

}

