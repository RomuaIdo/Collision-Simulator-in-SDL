#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct{ //Strcut do circulo
    float V; // Modulo da Velocidade
    double angle; // Angulo da velocidade
    float x, y;
    float vx, vy;
    int collisionx, collisiony;
    float radius;
} Ball;

extern Ball ball;

#endif