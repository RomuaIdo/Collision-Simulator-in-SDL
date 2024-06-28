//Arquivo de constantes e variaveis do jogo
#ifndef GLOBALS_H
#define GLOBALS_H
#include <SDL2/SDL.h>
//Constantes
#define SCREEN_WIDTH 800 //Largura da tela
#define SCREEN_HEIGHT 600 //Altura da tela
#define FALSE 0 //Falso
#define TRUE 1 //Verdadeiro
#define FPS 60 //Frames por segundo
#define FRAME_TARGET_TIME (1000 / FPS) //Tempo de um frame

// Variáveis Globais
extern int last_frame_time; // Tempo do último frame
extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern int running;
extern float CR;

#endif