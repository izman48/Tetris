#version 120

uniform vec2 windowSize; // size of given window
uniform vec2 position; // position of given square
uniform float blockSize; // size of given block 
uniform vec2 orthoWindowSize; // size of viewport

void main() {
    
    // covert position and block size to be in terms of screen space
    vec2 screenSpaceBlockSize = vec2(blockSize, blockSize)/orthoWindowSize;
    vec2 screenSpacePosition = position/orthoWindowSize;

    // difference between pixel and position
    vec2 difference = gl_FragCoord.xy/windowSize - screenSpacePosition;

    // difference as a percentage
    vec2 percentDif = difference / screenSpaceBlockSize;  
    
    // invert x 
    percentDif.x = 1.0 - percentDif.x;

    
    // calculates shading multiplier for each square
    float multiplier = (1-0.5*int(percentDif.x < 0.1) + 0.5*int(percentDif.x > 0.9)) *  (1 - 0.5*int(percentDif.y < 0.1) + 0.5*int(percentDif.y > 0.9));

    // set fragment color
    gl_FragColor = gl_Color * multiplier;
    
}