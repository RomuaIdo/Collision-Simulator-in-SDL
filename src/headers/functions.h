//Arquivo de cabeçalho com as funções utilizadas no programa

#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <SDL2/SDL_mixer.h>

//FUNCOES SETUPS
Mix_Chunk* loadSound(const char* path);
int initialize(void);
void setup_initial_screen(void);
void setup();
int main(void);
// FUNCOES DE PROCESSAMENTO
void process_initial_screen_input(void);
void process_input(void);
void update(void);
void update_positions(float delta_time);
// FUNCOES DE RENDERIZAÇÃO
void render_initial_screen(void);
void render(void);
void fill_triangle(SDL_Renderer *renderer, SDL_Point* p);
// FUNCOES DE DESTRUIÇÃO
void destroy_window(void);
void free_alocatedmemory(void);
// FUNCAO PARA DESENHAR UM CIRCULO
int SDL_RenderFillCircle(SDL_Renderer * renderer, int x, int y, int radius);
int SDL_RenderDrawCircle(SDL_Renderer * renderer, int x, int y, int radius);
#endif