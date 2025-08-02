#ifndef MAIN_H
#define MAIN_H
#include "utils.h"
#include "state.h"
#include "physics.h"
#include "input.h"
#include "objects.h"

int initialize(void);
void setup_initial_screen(void);
void setup();
int main(int argc, char *argv[]);
void free_alocatedmemory(void);
#endif
