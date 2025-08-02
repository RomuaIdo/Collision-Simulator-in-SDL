#ifndef INPUT_H
#define INPUT_H
#include "state.h"
#include "utils.h"

typedef struct {
  int mass_center;
  int vectors;
  int gravity;
  int mute;
} ShowRender;



void process_input(void);
void process_initial_screen_input(void);

#endif
