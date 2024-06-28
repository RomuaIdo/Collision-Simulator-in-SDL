#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct{ //Strcut do retângulo
    float x, y;
    float vx, vy;
    float width, height;
    int collisionx, collisiony;
    float radius;
} Rectangle;

extern Rectangle rect;

#endif