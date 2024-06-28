//Arquivo de cabeçalho com as funções utilizadas no programa

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

//FUNCOES SETUPS
int initialize_window(void);
void setup();
int main(void);
// FUNCOES DE PROCESSAMENTO
void process_input(void);
void update(void);
// FUNCOES DE RENDERIZAÇÃO
void render(void);
// FUNCOES DE DESTRUIÇÃO
void destroy_window(void);
// FUNCAO PARA DESENHAR UM CIRCULO
int SDL_RenderFillCircle(SDL_Renderer * renderer, int x, int y, int radius);
int SDL_RenderDrawCircle(SDL_Renderer * renderer, int x, int y, int radius);
#endif