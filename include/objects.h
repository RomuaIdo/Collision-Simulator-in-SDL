#ifndef OBJECTS_H
#define OBJECTS_H
#include "physics.h"

typedef struct BallNode {
  Ball ball;
  struct BallNode *next;
} BallNode;

void shuffle_balls(void);
void add_ball(Ball ball);
void add_random_ball(void);
void remove_last_ball(void);
Ball generate_random_ball(void);

#endif
