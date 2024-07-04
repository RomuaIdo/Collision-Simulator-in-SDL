//Arquivo de constantes e variaveis do jogo
#ifndef GLOBALS_H
#define GLOBALS_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "./structs.h"

// Variáveis Globais
extern int last_frame_time; // Tempo do último frame
extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern int running;
extern float CR;
extern int n_balls;
extern State state;
extern Circle_Button *start_button;
extern Mix_Chunk *collision_sound;
extern Triangle *triangle;
//Array de ponteiros para as particulas
extern Ball **balls;
extern Border *border;
//Matriz de ponteiros de colisao entre particulas
extern Collision ***collisions;
extern SDL_Rect *box;
extern TTF_Font *font;
extern SDL_Texture *text_texture;
extern SDL_Rect* text_rect;
extern Texto *fonte;
extern char text[20];
extern ShowRender *show_render;
extern MassCenter *mass_center;
#endif