#ifndef PHYSICS_H
#define PHYSICS_H
#include "graphics.h"

#define GRAVITY 9.8 

typedef struct Simulator Simulator;
typedef struct BallArray BallArray;
typedef struct CollisionArray CollisionArray;

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

void update_physics_step(Simulator* simulator, float delta_time);
void detect_collisions(BallArray *ball_array, CollisionArray *collision_array);
void resolve_static_collisions(CollisionArray *collision_array);
void resolve_dynamic_collisions(Simulator *simulator, CollisionArray *collision_array);
void integrate_motion(Simulator *simulator, float delta_time);

void update_mass_center(MassCenter *mass_center, BallArray* ball_array);
void handle_wall_collision(Ball *b, Border *border_area, float restitution);

#endif
