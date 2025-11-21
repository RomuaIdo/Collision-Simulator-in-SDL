#ifndef OBJECTS_H
#define OBJECTS_H
#include "physics.h"

typedef struct Simulator Simulator;

typedef struct BallNode {
  Ball* ball;
  struct BallNode *next;
} BallNode;

void shuffle_balls(Simulator* simulator);
void add_ball(Simulator* simulator, Ball* ball);
void add_random_ball(Simulator* simulator);
void remove_last_ball(Simulator* simulator);
Ball* generate_random_ball(void);

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
