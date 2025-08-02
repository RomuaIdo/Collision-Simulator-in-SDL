#include "../include/objects.h"
#include "../include/input.h"

extern BallNode *balls;
extern int n_balls;
extern MassCenter *mass_center;
extern Border *border;
extern ShowRender *show_render;

BallNode *create_ball_node(Ball ball) {
  BallNode *new_node = (BallNode *)malloc(sizeof(BallNode));
  if (!new_node) {
    fprintf(stderr, "Error allocating memory for ball node.\n");
    exit(EXIT_FAILURE);
  }
  new_node->ball = ball;
  new_node->next = NULL;
  return new_node;
}

void add_ball(Ball ball) {
  BallNode *new_node = create_ball_node(ball);
  if (balls == NULL) {
    balls = new_node;
  } else {
    BallNode *current = balls;
    while (current->next != NULL) {
      current = current->next;
    }
    current->next = new_node;
  }
  n_balls++;
}

Ball generate_random_ball(void) {
  Ball ball;
  ball.mass = (((double)rand() / RAND_MAX) * 2) + 1;
  ball.radius = (int)(ball.mass * 15);
  ball.V = 400;
  ball.angle = ((double)rand() / RAND_MAX) * M_PI;
  ball.vx = ball.V * cos(ball.angle);
  ball.vy = ball.V * sin(ball.angle);
  ball.x = (double)rand() / RAND_MAX * (SCREEN_WIDTH - (3 * ball.radius)) +
           ball.radius;
  ball.y = (double)rand() / RAND_MAX * (SCREEN_HEIGHT - (3 * ball.radius)) +
           ball.radius;
  ball.color_r = rand() % 256;
  ball.color_g = rand() % 256;
  ball.color_b = rand() % 256;
  ball.color_a = 255;
  return ball;
}

void add_random_ball() {
  Ball new_ball;
  new_ball.mass = ((float)rand() / RAND_MAX) * 2.0f + 1.0f;
  new_ball.radius = (int)(new_ball.mass * 15);
  new_ball.V = 400.0f;
  new_ball.angle = ((float)rand() / RAND_MAX) * 2 * M_PI;
  new_ball.vx = new_ball.V * cos(new_ball.angle);
  new_ball.vy = new_ball.V * sin(new_ball.angle);
  new_ball.x = (float)rand() / RAND_MAX *
                   (border->x2 - border->x1 - 2 * new_ball.radius) +
               border->x1 + new_ball.radius;
  new_ball.y = (float)rand() / RAND_MAX *
                   (border->y2 - border->y1 - 2 * new_ball.radius) +
               border->y1 + new_ball.radius;
  new_ball.color_r = rand() % 256;
  new_ball.color_g = rand() % 256;
  new_ball.color_b = rand() % 256;
  new_ball.color_a = 255;

  BallNode *new_node = (BallNode *)malloc(sizeof(BallNode));
  new_node->ball = new_ball;
  new_node->next = NULL;

  if (!balls) {
    balls = new_node;
  } else {
    BallNode *current = balls;
    while (current->next) {
      current = current->next;
    }
    current->next = new_node;
  }

  n_balls++;
  mass_center->total_mass += new_ball.mass;
}

void remove_last_ball() {
  if (!balls)
    return;

  if (!balls->next) {
    free(balls);
    balls = NULL;
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
    mass_center->total_mass -= current->ball.mass;
    free(current);
  }

  n_balls--;
  if (show_render->mass_center && n_balls > 0) {
    update_mass_center();
  }
}

void shuffle_balls(void) {
  BallNode *current = balls;
  while (current) {
    Ball *b = &(current->ball);
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

  if (show_render->mass_center) {
    update_mass_center();
  }
}
