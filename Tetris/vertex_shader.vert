#version 120
void main() {
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex; // calculate gl position 
    gl_FrontColor = gl_Color; // set gl_FrontColor as gl_color

}