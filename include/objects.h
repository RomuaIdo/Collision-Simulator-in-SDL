#ifndef OBJECTS_H
#define OBJECTS_H
#include "physics.h"

typedef struct Simulator Simulator;

typedef struct BallArray {
  Ball* data;
  int count;
  int capacity;
} BallArray;

void init_ball_array(BallArray* ball_array, int initial_capacity);
void shuffle_balls(BallArray* ball_array, MassCenter* mass_center, Border* border);
void add_ball(BallArray* ball_array, Ball ball);
void add_random_ball(BallArray* ball_array, Border* border);
void remove_last_ball(BallArray* ball_array, MassCenter* mass_center);
Ball generate_random_ball(Border* border);
void free_ball_array(BallArray* ball_array);

typedef struct CollisionArray {
  CollisionData *data;
  int count;
  int capacity;
} CollisionArray;

void init_collision_array(CollisionArray *array, int initial_capacity);
void add_collision(CollisionArray *array, CollisionData collision);
void clear_collision_array(CollisionArray *array);
void free_collision_array(CollisionArray *array);

#endif
