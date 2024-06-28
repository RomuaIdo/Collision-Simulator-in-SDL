#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct{ //Strcut do circulo
    float V; // Modulo da Velocidade
    double angle; // Angulo da velocidade
    float x, y;
    float vx, vy;
    int collision_wallx, collision_wally;
    float radius;
    unsigned char color_r, color_g, color_b, color_a;
} Ball;

#endif