#include "../include/functions.h"
#include "../include/globals.h"
#include "../include/macros.h"
#include "../include/structs.h"

#define MAX_COLLISIONS 1000

int collision_wallx = FALSE;
int collision_wally = FALSE;

void process_initial_screen_input(void) {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      running = FALSE;
      break;

    case SDL_MOUSEBUTTONDOWN:
      if (event.button.button == SDL_BUTTON_LEFT) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        if (sqrt(pow(x - start_button->x, 2) + pow(y - start_button->y, 2)) <=
            start_button->radius) {
          state = RUNNING;
          *border = (Border){55, 55, (BOX_FACTOR_X * SCREEN_WIDTH) - 5,
                             (BOX_FACTOR_Y * SCREEN_HEIGHT) - 5};
          sprintf(text, "Bola: %d", 0);
          shuffle_balls();
        }
      }
      break;

    case SDL_KEYDOWN:
      switch (event.key.keysym.sym) {
      case SDLK_ESCAPE:
        running = FALSE;
        break;
      }
      break;
    }
  }
}

void process_input(void) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      running = FALSE;
      break;

    case SDL_KEYDOWN:
      switch (event.key.keysym.sym) {
      case SDLK_ESCAPE:
        running = FALSE;
        break;

      case SDLK_p:
        if (state == RUNNING) {
          state = PAUSED;
        } else if (state == PAUSED) {
          state = RUNNING;
        }
        break;
      case SDLK_s:
        shuffle_balls();
        break;

      case SDLK_g:
        show_render->gravity = !show_render->gravity;
        break;

      case SDLK_c:
        show_render->mass_center = !show_render->mass_center;
        break;

      case SDLK_v:
        show_render->vectors = !show_render->vectors;
        break;

      case SDLK_m:
        show_render->mute = !show_render->mute;
        break;

      case SDLK_PLUS:
      case SDLK_KP_PLUS:
        add_random_ball();
        break;

      case SDLK_MINUS:
      case SDLK_KP_MINUS:
        remove_last_ball();
        break;
      }
      break;
    }
  }
}

void update(void) {
  float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;
  last_frame_time = SDL_GetTicks();

  int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);
  if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
    SDL_Delay(time_to_wait);
  }

  if (state != PAUSED) {
    update_positions(delta_time);
  }
}

void update_positions(float delta_time) {
  CollisionData collisions[MAX_COLLISIONS];
  int collision_count = 0;

  BallNode *node_i = balls;
  while (node_i) {
    Ball *a = &(node_i->ball);
    BallNode *node_j = node_i->next;

    while (node_j) {
      Ball *b = &(node_j->ball);
      float dx = b->x - a->x;
      float dy = b->y - a->y;
      float distance = sqrt(dx * dx + dy * dy);
      float min_distance = a->radius + b->radius;

      if (distance < min_distance) {
        if (collision_count < MAX_COLLISIONS) {
          collisions[collision_count].a = a;
          collisions[collision_count].b = b;
          collisions[collision_count].nx = dx / distance;
          collisions[collision_count].ny = dy / distance;
          collisions[collision_count].overlap = min_distance - distance;
          collision_count++;
        }
      }
      node_j = node_j->next;
    }
    node_i = node_i->next;
  }

  for (int i = 0; i < collision_count; i++) {
    Ball *a = collisions[i].a;
    Ball *b = collisions[i].b;
    const float nx = collisions[i].nx;
    const float ny = collisions[i].ny;
    const float overlap = collisions[i].overlap;

    const float displace = overlap * 0.5f;
    a->x -= nx * displace;
    a->y -= ny * displace;
    b->x += nx * displace;
    b->y += ny * displace;
  }

  for (int i = 0; i < collision_count; i++) {
    Ball *a = collisions[i].a;
    Ball *b = collisions[i].b;
    const float nx = collisions[i].nx;
    const float ny = collisions[i].ny;

    float vrel = (b->vx - a->vx) * nx + (b->vy - a->vy) * ny;
    float impulse = -(1 + CR) * vrel / (1 / a->mass + 1 / b->mass);

    a->vx -= impulse * nx / a->mass;
    a->vy -= impulse * ny / a->mass;
    b->vx += impulse * nx / b->mass;
    b->vy += impulse * ny / b->mass;

    a->V = sqrt(a->vx * a->vx + a->vy * a->vy);
    a->angle = atan2(a->vy, a->vx);
    b->V = sqrt(b->vx * b->vx + b->vy * b->vy);
    b->angle = atan2(b->vy, b->vx);

    if (state == RUNNING && !show_render->mute) {
      Mix_VolumeChunk(collision_sound, (int)fabs(impulse));
      Mix_PlayChannel(-1, collision_sound, 0);
    }
  }

  BallNode *current = balls;
  while (current) {
    Ball *b = &(current->ball);

    b->x += b->vx * delta_time;
    b->y += b->vy * delta_time;

    handle_wall_collision(b, border, CR);

    if (show_render->gravity) {
      b->vy += GRAVITY;
    }

    current = current->next;
  }

  if (show_render->mass_center) {
    update_mass_center();
  }
}

void update_mass_center(void) {
  float x = 0, y = 0, vx = 0, vy = 0, total_mass = 0;

  BallNode *current = balls;
  while (current) {
    Ball *b = &(current->ball);
    x += b->x * b->mass;
    y += b->y * b->mass;
    vx += b->vx * b->mass;
    vy += b->vy * b->mass;
    total_mass += b->mass;
    current = current->next;
  }

  if (total_mass > 0) {
    mass_center->vx = vx / total_mass;
    mass_center->vy = vy / total_mass;
    mass_center->x = x / total_mass;
    mass_center->y = y / total_mass;
    mass_center->total_mass = total_mass;
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
  new_ball.collision_wallx = FALSE;
  new_ball.collision_wally = FALSE;
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

void handle_wall_collision(Ball *b, Border *border_area, float restitution) {
  if (b->x - b->radius < border_area->x1) {
    b->x = border_area->x1 + b->radius;
    b->vx = fabs(b->vx) * restitution;
  } else if (b->x + b->radius > border_area->x2) {
    b->x = border_area->x2 - b->radius;
    b->vx = -fabs(b->vx) * restitution;
  }

  if (b->y - b->radius < border_area->y1) {
    b->y = border_area->y1 + b->radius;
    b->vy = fabs(b->vy) * restitution;
  } else if (b->y + b->radius > border_area->y2) {
    b->y = border_area->y2 - b->radius;
    b->vy = -fabs(b->vy) * restitution;
  }
}
