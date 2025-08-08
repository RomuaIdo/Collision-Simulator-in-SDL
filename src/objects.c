#include "../include/objects.h"
#include "../include/input.h"
#include "../include/main.h"

BallNode *create_ball_node(Ball* ball) {
  BallNode *new_node = (BallNode *)malloc(sizeof(BallNode));
  if (!new_node) {
    fprintf(stderr, "Error allocating memory for ball node.\n");
    exit(EXIT_FAILURE);
  }
  new_node->ball = ball;
  new_node->next = NULL;
  return new_node;
}

void add_ball(Simulator* simulator, Ball* ball) {
  BallNode *new_node = create_ball_node(ball);
  if (simulator->balls == NULL) {
    simulator->balls = new_node;
  } else {
    BallNode *current = simulator->balls;
    while (current->next != NULL) {
      current = current->next;
    }
    current->next = new_node;
  }
  simulator->n_balls++;
  simulator->mass_center->total_mass += ball->mass;
}

Ball* generate_random_ball(void) {
  Ball* ball;
  ball = (Ball *)malloc(sizeof(Ball));
  ball->mass = (((double)rand() / RAND_MAX) * 2) + 1;
  ball->radius = (int)(ball->mass * 15);
  ball->V = 400;
  ball->angle = ((double)rand() / RAND_MAX) * M_PI;
  ball->vx = ball->V * cos(ball->angle);
  ball->vy = ball->V * sin(ball->angle);
  ball->x = (double)rand() / RAND_MAX * (SCREEN_WIDTH - (3 * ball->radius)) +
           ball->radius;
  ball->y = (double)rand() / RAND_MAX * (SCREEN_HEIGHT - (3 * ball->radius)) +
           ball->radius;
  ball->color_r = rand() % 256;
  ball->color_g = rand() % 256;
  ball->color_b = rand() % 256;
  ball->color_a = 255;
  return ball;
}

void add_random_ball(Simulator* simulator) {
  Ball* ball = generate_random_ball();
  add_ball(simulator, ball);
}

void remove_last_ball(Simulator* simulator) {
  BallNode *balls = simulator->balls;
  if (!balls)
    return;

  if (!balls->next) {
    free(balls->ball);
    free(balls);
    simulator->balls = NULL;
  } else {
    BallNode *current = balls;
    BallNode *prev = NULL;

    while (current->next) {
      prev = current;
      current = current->next;
    }

    if (prev) {
      prev->next = NULL;
    }
    simulator->mass_center->total_mass -= current->ball->mass;
    free(current->ball);
    free(current);
  }

  simulator->n_balls--;
  if (simulator->settings->mass_center && simulator->n_balls > 0) {
    update_mass_center(simulator);
  }
}

void shuffle_balls(Simulator* simulator) {
  Settings *settings = simulator->settings;
  Border *border = simulator->border;
  BallNode *current = simulator->balls;
  while (current) {
    Ball *b = current->ball;
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
    current = current->next;
  }

  if (settings->mass_center) {
    update_mass_center(simulator);
  }
}
