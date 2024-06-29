#ifndef STRUCTS_H
#define STRUCTS_H

typedef enum{
    INITIAL_SCREEN,
    PROCESSING,
    RUNNING
} State;

typedef struct{
    int x, y;
    int radius;
    unsigned char r, g, b, a;
} Circle_Button;

typedef struct{ //Strcut do circulo
    float V; // Modulo da Velocidade
    double angle; // Angulo da velocidade
    float x, y;
    float vx, vy;
    int collision_wallx, collision_wally;
    int radius;
    float mass;
    float impulse;
    unsigned char color_r, color_g, color_b, color_a;
} Ball;

typedef struct{
    int collision;
    double angle;
    float nx;
    float ny;
    float vrel_n;
    float impulse_n;
    float vcm_x;
    float vcm_y;
    float overlap;
    float distance;
}Collision;

#endif