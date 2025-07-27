#ifndef STRUCTS_H
#define STRUCTS_H
#include <SDL2/SDL.h>

typedef enum { INITIAL_SCREEN, PROCESSING, RUNNING, PAUSED } State;

typedef struct {
  int x, y;
  double vx, vy;
  double total_mass;
  int radius;
} MassCenter;

typedef struct {
  int mass_center;
  int vectors;
  int gravity;
  int mute;
} ShowRender;

typedef struct {
  SDL_Color color;
  int size;
} Texto;

typedef struct {
  int x1, y1;
  int x2, y2;
} Border;

typedef struct {
  double x, y;
  double radius;
  unsigned char r, g, b, a;
} Circle_Button;

typedef struct {
  SDL_Point points[3];
  unsigned char r, g, b, a;
} Triangle;

typedef struct {
  float V;
  double angle;
  float x, y;
  float vx, vy;
  int collision_wallx, collision_wally;
  int radius;
  float mass;
  unsigned char color_r, color_g, color_b, color_a;
} Ball;

typedef struct {
  int collision;
  double angle;
  float nx;
  float ny;
  float vrel_n;
  float impulse_n;
  float vcm_x;
  float vcm_y;
  float overlap;
  float distance;
} Collision;

typedef struct {
  Ball *a;
  Ball *b;
  float nx, ny;
  float overlap;
} CollisionData;

typedef struct BallNode {
  Ball ball;
  struct BallNode *next;
} BallNode;

#endif
