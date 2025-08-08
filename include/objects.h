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

#endif
