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
// FUNCOES DE RENDERIZAÇÃO
void render_initial_screen(void);
void render(void);
// FUNCOES DE DESTRUIÇÃO
void destroy_window(void);
// FUNCAO PARA DESENHAR UM CIRCULO
int SDL_RenderFillCircle(SDL_Renderer * renderer, int x, int y, int radius);
int SDL_RenderDrawCircle(SDL_Renderer * renderer, int x, int y, int radius);
#endif