#include "../include/objects.h"
#include "../include/physics.h"

void init_ball_array(BallArray *ball_array, int initial_capacity) {
  ball_array->data = (Ball *)malloc(initial_capacity * sizeof(Ball));
  if (!ball_array->data) {
    fprintf(stderr, "Error allocating memory for ball array.\n");
    exit(EXIT_FAILURE);
  }
  ball_array->count = 0;
  ball_array->capacity = initial_capacity;
}

void add_ball(BallArray *ball_array, Ball ball) {
  if (ball_array->count >= ball_array->capacity) {
    ball_array->capacity *= 2;
    ball_array->data =
        (Ball *)realloc(ball_array->data, ball_array->capacity * sizeof(Ball));
    if (!ball_array->data) {
      fprintf(stderr, "Error reallocating memory for ball array.\n");
      exit(EXIT_FAILURE);
    }
  }
  ball_array->data[ball_array->count++] = ball;
}

Ball generate_random_ball(Border *border) {
  Ball ball;
  ball.mass = (((double)rand() / RAND_MAX) * 2) + 1;
  ball.radius = (int)(ball.mass * 15);
  ball.V = 400;
  ball.angle = ((double)rand() / RAND_MAX) * M_PI;
  ball.vx = ball.V * cos(ball.angle);
  ball.vy = ball.V * sin(ball.angle);
  ball.x = (double)rand() / RAND_MAX * (border->x2 - border->x1 - (2 * ball.radius)) +
           ball.radius + border->x1;
  ball.y = (double)rand() / RAND_MAX * (border->y2 - border->y1 - (2 * ball.radius)) +
           ball.radius + border->y1;
  ball.color_r = rand() % 256;
  ball.color_g = rand() % 256;
  ball.color_b = rand() % 256;
  ball.color_a = 255;
  return ball;
}

void add_random_ball(BallArray *ball_array, Border *border) {
  Ball ball = generate_random_ball(border);
  add_ball(ball_array, ball);
}

void remove_last_ball(BallArray *ball_array, MassCenter *mass_center) {
  if (ball_array->count > 0) {
    ball_array->count--;
    if (mass_center) {
      mass_center->total_mass -= ball_array->data[ball_array->count].mass;
      update_mass_center(mass_center, ball_array);
    }
  }
}

void shuffle_balls(BallArray *ball_array, MassCenter *mass_center,
                   Border *border) {
  for (int i = 0; i < ball_array->count; i++) {
    Ball *b = &ball_array->data[i];
    b->x =
        (float)rand() / RAND_MAX * (border->x2 - border->x1 - 2 * b->radius) +
        border->x1 + b->radius;
    b->y =
        (float)rand() / RAND_MAX * (border->y2 - border->y1 - 2 * b->radius) +
        border->y1 + b->radius;
    b->V = 400;
    b->angle = ((float)rand() / RAND_MAX) * 2 * M_PI;
    b->vx = b->V * cos(b->angle);
    b->vy = b->V * sin(b->angle);
  }

  if (mass_center) {
    update_mass_center(mass_center, ball_array);
  }
}

void free_ball_array(BallArray *ball_array) {
  free(ball_array->data);
  ball_array->data = NULL;
  ball_array->count = 0;
  ball_array->capacity = 0;
}


void init_collision_array(CollisionArray *array, int initial_capacity) {
  array->data =
      (CollisionData *)malloc(initial_capacity * sizeof(CollisionData));
  if (!array->data) {
    fprintf(stderr, "Error allocating memory for collision array.\n");
    exit(EXIT_FAILURE);
  }
  array->count = 0;
  array->capacity = initial_capacity;
}

void add_collision(CollisionArray *array, CollisionData collision) {
  if (array->count >= array->capacity) {
    array->capacity *= 2;
    array->data = (CollisionData *)realloc(
        array->data, array->capacity * sizeof(CollisionData));
    if (!array->data) {
      fprintf(stderr, "Error reallocating memory for collision array.\n");
      exit(EXIT_FAILURE);
    }
  }
  array->data[array->count++] = collision;
}

void clear_collision_array(CollisionArray *array) { array->count = 0; }

void free_collision_array(CollisionArray *array) {
  free(array->data);
  array->data = NULL;
  array->count = 0;
  array->capacity = 0;
}
