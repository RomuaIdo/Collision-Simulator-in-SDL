#ifndef INPUT_H
#define INPUT_H
#include "state.h"
#include "utils.h"

typedef struct Simulator Simulator;
typedef struct MainMenu MainMenu;
typedef struct PhysicsWorld PhysicsWorld;

typedef struct Settings {
  int mass_center;
  int vectors;
  int gravity;
  int mute;
} Settings;


void process_input(Settings* settings, PhysicsWorld* world, State* state, int* running);
void process_initial_screen_input(MainMenu* menu, PhysicsWorld* world, State* state, int* running);

#endif
