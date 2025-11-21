#include "../include/physics.h"
#include "../include/objects.h"
#include "../include/input.h"
#include "../include/main.h"

static void apply_impulse(Ball *a, Ball *b, float nx, float ny, float cr);

void update(Simulator *simulator) {
  float delta_time = (SDL_GetTicks() - simulator->last_frame_time) / 1000.0f;
  simulator->last_frame_time = SDL_GetTicks();

  int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - simulator->last_frame_time);
  if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
    SDL_Delay(time_to_wait);
  }

  if (simulator->state != PAUSED) {
    update_physics_step(simulator, delta_time);
  }
}

void update_physics_step(Simulator *simulator, float delta_time) {
  clear_collision_array(&simulator->collision_array);
  

  detect_collisions(&simulator->ball_array, &simulator->collision_array);

  resolve_static_collisions(&simulator->collision_array);

  resolve_dynamic_collisions(simulator, &simulator->collision_array);

  integrate_motion(simulator, delta_time);

  if (simulator->settings->mass_center) {
    update_mass_center(simulator->mass_center, &simulator->ball_array);
  }
}

void detect_collisions(BallArray *ball_array, CollisionArray* collision_array) {
  for (int i = 0; i < ball_array->count; i++) {
    Ball *a = &ball_array->data[i];
    for (int j = i + 1; j < ball_array->count; j++) {
      Ball *b = &ball_array->data[j];
      
      float dx = b->x - a->x;
      float dy = b->y - a->y;
      float dist_sq = dx * dx + dy * dy;
      float radius_sum = a->radius + b->radius;
      
      if (dist_sq < radius_sum * radius_sum) {
        float dist = sqrtf(dist_sq);
        float overlap = radius_sum - dist;
        
        float nx = dx / dist;
        float ny = dy / dist;
        
        CollisionData collision;
        collision.a = a;
        collision.b = b;
        collision.nx = nx;
        collision.ny = ny;
        collision.overlap = overlap;
        
        add_collision(collision_array, collision);
      }
    }
  }
}

void resolve_static_collisions(CollisionArray *collision_array) {
  for (int i = 0; i < collision_array->count; i++) {
    Ball *a = collision_array->data[i].a;
    Ball *b = collision_array->data[i].b;
    
    const float displace = collision_array->data[i].overlap * 0.5f;
    
    a->x -= collision_array->data[i].nx * displace;
    a->y -= collision_array->data[i].ny * displace;
    b->x += collision_array->data[i].nx * displace;
    b->y += collision_array->data[i].ny * displace;
  }
}

void resolve_dynamic_collisions(Simulator *simulator, CollisionArray *collision_array) {
  for (int i = 0; i < collision_array->count; i++) {
    Ball *a = collision_array->data[i].a;
    Ball *b = collision_array->data[i].b;
    
    apply_impulse(a, b, collision_array->data[i].nx, collision_array->data[i].ny, simulator->CR);

    a->V = sqrt(a->vx * a->vx + a->vy * a->vy);
    a->angle = atan2(a->vy, a->vx);
    b->V = sqrt(b->vx * b->vx + b->vy * b->vy);
    b->angle = atan2(b->vy, b->vx);

    if (simulator->state == RUNNING && !simulator->settings->mute) {
      float vrel_sq = pow(b->vx - a->vx, 2) + pow(b->vy - a->vy, 2);
      int volume = (int)(sqrt(vrel_sq) / 5); 
      if (volume > 128) volume = 128;
      
      Mix_VolumeChunk(simulator->collision_sound, volume);
      Mix_PlayChannel(-1, simulator->collision_sound, 0);
    }
  }
}

static void apply_impulse(Ball *a, Ball *b, float nx, float ny, float cr) {
    float vrel = (b->vx - a->vx) * nx + (b->vy - a->vy) * ny;

    if (vrel > 0) return;

    float impulse = -(1 + cr) * vrel / (1 / a->mass + 1 / b->mass);

    a->vx -= impulse * nx / a->mass;
    a->vy -= impulse * ny / a->mass;
    b->vx += impulse * nx / b->mass;
    b->vy += impulse * ny / b->mass;
}

void integrate_motion(Simulator* simulator, float delta_time) {
  
  for (int i = 0; i < simulator->ball_array.count; i++) {
    Ball *b = &simulator->ball_array.data[i];

    if (simulator->settings->gravity) {
      b->vy += GRAVITY; // Gravidade constante por frame ou multiplicar por delta_time se quiser simulação precisa SI
    }

    b->x += b->vx * delta_time;
    b->y += b->vy * delta_time;

    handle_wall_collision(b, simulator->border, simulator->CR);

  }
}

void update_mass_center(MassCenter* mass_center, BallArray* ball_array) {
  float x = 0, y = 0, vx = 0, vy = 0, total_mass = 0;

  for (int i = 0; i < ball_array->count; i++) {
    Ball *b = &ball_array->data[i];
    x += b->x * b->mass;
    y += b->y * b->mass;
    vx += b->vx * b->mass;
    vy += b->vy * b->mass;
    total_mass += b->mass;
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
  int hit = 0;
  
  if (b->x - b->radius < border_area->x1) {
    b->x = border_area->x1 + b->radius;
    b->vx = fabs(b->vx) * restitution;
    hit = 1;
  } else if (b->x + b->radius > border_area->x2) {
    b->x = border_area->x2 - b->radius;
    b->vx = -fabs(b->vx) * restitution;
    hit = 1;
  }

  if (b->y - b->radius < border_area->y1) {
    b->y = border_area->y1 + b->radius;
    b->vy = fabs(b->vy) * restitution;
    hit = 1;
  } else if (b->y + b->radius > border_area->y2) {
    b->y = border_area->y2 - b->radius;
    b->vy = -fabs(b->vy) * restitution;
    hit = 1;
  }
}
