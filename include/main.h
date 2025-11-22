#ifndef MAIN_H
#define MAIN_H
#include "graphics.h"
#include "physics.h"
#include "input.h"
#include "objects.h"
#include "state.h"
#include "utils.h"

typedef struct MainMenu {
  Text* titulo;
  Circle_Button* start_button;
  Triangle* triangle;
} MainMenu;

typedef struct Resources {
  Mix_Chunk *collision_sound;
  TTF_Font *font;
} Resources;


typedef struct Simulator {
  State state;
  int running;
  Settings* settings;
  int last_frame_time;

  WindowConfig window_config;
  PhysicsWorld world;
  MainMenu menu;
  Resources resources;
} Simulator;

int initialize(Simulator *simulator);
void setup_initial_screen(Simulator *simulator);
void setup(Simulator *simulator);
int main(int argc, char *argv[]);
void free_alocatedmemory(Simulator *simulator);
#endif
