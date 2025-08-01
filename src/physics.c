#include "../include/physics.h"
#include "../include/utils.h"
#include "../include/objects.h"
#include "../include/input.h"

int collision_wallx = FALSE;
int collision_wally = FALSE;

extern int last_frame_time;
extern BallNode *balls;
extern Border *border;
extern ShowRender *show_render;
extern MassCenter *mass_center;
extern Mix_Chunk *collision_sound;
extern State state;
extern float CR;

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
