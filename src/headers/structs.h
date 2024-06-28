#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct{ //Strcut do retângulo
    float x, y;
    float vx, vy;
    int collisionx, collisiony;
    float radius;
} Ball;

extern Ball ball;

#endif