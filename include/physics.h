#ifndef PHYSICS_H
#define PHYSICS_H
#include "graphics.h"

#define GRAVITY 9.8 
#define MAX_COLLISIONS 1000

typedef struct Simulator Simulator;

typedef struct {
  float V;
  float angle;
  float x, y;
  float vx, vy;
  int radius;
  float mass;
  unsigned char color_r, color_g, color_b, color_a;
} Ball;

typedef struct {
  Ball *a;
  Ball *b;
  float nx, ny;
  float overlap;
} CollisionData;

typedef struct {
  int x, y;
  double vx, vy;
  double total_mass;
  int radius;
} MassCenter;

void update(Simulator *simulator);
void update_mass_center(Simulator *simulator);
void update_positions(Simulator *simulator, float delta_time);
void handle_wall_collision(Ball *b, Border *border_area, float restitution);

#endif
