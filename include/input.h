#ifndef INPUT_H
#define INPUT_H
#include "state.h"
#include "utils.h"

typedef struct Simulator Simulator;

typedef struct {
  int mass_center;
  int vectors;
  int gravity;
  int mute;
} Settings;



void process_input(Simulator* simulator);
void process_initial_screen_input(Simulator* simulator);

#endif
