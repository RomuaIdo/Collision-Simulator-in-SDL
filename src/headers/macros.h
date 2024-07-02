#ifndef MACROS_H
#define MACROS_H

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <math.h>
#include <time.h>

//Constantes
#define SCREEN_WIDTH 1200 //Largura da tela
#define SCREEN_HEIGHT 900 //Altura da tela
#define FALSE 0 //Falso
#define TRUE 1 //Verdadeiro
#define FPS 60 //Frames por segundo
#define FRAME_TARGET_TIME (1000 / FPS) //Tempo de um frame
#ifndef M_PI
#define M_PI		3.14159265358979323846    /* pi */
#endif
#define TRIANGLE_FACTOR 0.8 //Fator de escala do triangulo
#define BORDER_FACTOR 0.9 //Fator de escala da borda

#endif