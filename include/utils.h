#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#define FALSE 0
#define TRUE 1


#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif


Mix_Chunk* loadSound(const char* path);


#endif
