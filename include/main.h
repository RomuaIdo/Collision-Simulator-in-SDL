#ifndef MAIN_H
#define MAIN_H
#include "graphics.h"
#include "physics.h"
#include "input.h"
#include "objects.h"
#include "state.h"
#include "utils.h"

typedef struct BallNode BallNode;

typedef struct Simulator {
  State state;
  int running;
  SDL_Window *window;
  SDL_Renderer *renderer;
  Settings* settings;
  MassCenter* mass_center;
  CollisionArray collision_array;
  float CR;
  Mix_Chunk *collision_sound;
  int last_frame_time;
  Border* border;
  SDL_Rect* box;
  Texto* titulo;
  Circle_Button* start_button;
  Triangle* triangle;
  SDL_Rect* text_rect;
  BallArray ball_array;
  char text[20];
  TTF_Font *font;
  SDL_Texture *text_texture;
} Simulator;

int initialize(Simulator *simulator);
void setup_initial_screen(Simulator *simulator);
void setup(Simulator *simulator);
int main(int argc, char *argv[]);
void free_alocatedmemory(Simulator *simulator);
#endif
