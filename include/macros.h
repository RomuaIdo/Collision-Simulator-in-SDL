#ifndef MACROS_H
#define MACROS_H

#include <stdio.h>
#include <stdlib.h>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <math.h>
#include <time.h>
#include <SDL2/SDL_ttf.h>

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 900
#define FALSE 0
#define TRUE 1
#define FPS 60
#define FRAME_TARGET_TIME (1000 / FPS)
#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif
#define TRIANGLE_FACTOR 0.8
#define BOX_FACTOR_X 0.75
#define BOX_FACTOR_Y 0.8 
#define GRAVITY 9.8 
#endif
