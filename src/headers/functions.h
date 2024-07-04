//Arquivo de cabeçalho com as funções utilizadas no programa

#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <SDL2/SDL_mixer.h>

//FUNCOES SETUPS
Mix_Chunk* loadSound(const char* path);
int initialize(void);
void setup_initial_screen(void);
void setup();
int main(int argc, char *argv[]);
// FUNCOES DE PROCESSAMENTO
void process_initial_screen_input(void);
void process_input(void);
void update(void);
void update_positions(float delta_time);
void shuffle_balls(void);
// FUNCOES DE RENDERIZAÇÃO
void render(void);
void fillTriangle(SDL_Renderer *renderer, SDL_Point* p);
void draw_box(SDL_Renderer *renderer, SDL_Rect *rect, int thickness);
// FUNCOES DE DESTRUIÇÃO
void destroy_window(void);
void free_alocatedmemory(void);
// FUNCAO PARA DESENHAR UM CIRCULO
int SDL_RenderFillCircle(SDL_Renderer * renderer, int x, int y, int radius);
int SDL_RenderDrawCircle(SDL_Renderer * renderer, int x, int y, int radius);
// FUNCAO PARA DESENHAR UM TEXTO
void load_font(const char* font_path, int font_size);
void create_text_texture(SDL_Renderer* renderer, const char* text, SDL_Color color);
#endif