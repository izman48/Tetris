#ifndef _SQUARE_H
#define _SQUARE_H

#define SIZE 30.f

class Square {
    float red;
    float green;
    float blue;


    public:
        Square(float x_, float y_, float r_, float g_, float b_);
        Square();
        void display();
        
        bool active;
        float x;
        float y;

};
#endif

