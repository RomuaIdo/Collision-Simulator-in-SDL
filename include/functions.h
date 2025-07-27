#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL.h>
#include "structs.h"
Mix_Chunk* loadSound(const char* path);
int initialize(void);
void setup_initial_screen(void);
void setup();
int main(int argc, char *argv[]);
void process_initial_screen_input(void);
void process_input(void);
void update(void);
void update_mass_center(void);
void update_positions(float delta_time);
void shuffle_balls(void);
void render(void);
void fillTriangle(SDL_Renderer *renderer, SDL_Point* p);
void draw_box(SDL_Renderer *renderer, SDL_Rect *rect, int thickness);
void draw_arrow(SDL_Renderer* renderer, int x1, int y1, int x2, int y2);
void destroy_window(void);
void free_alocatedmemory(void);
int SDL_RenderFillCircle(SDL_Renderer * renderer, int x, int y, int radius);
int SDL_RenderDrawCircle(SDL_Renderer * renderer, int x, int y, int radius);
void load_font(const char* font_path, int font_size);
void create_text_texture(SDL_Renderer* renderer, const char* text, SDL_Color color);
BallNode* create_ball(float x, float y, float radius);
void add_ball(Ball ball);
void remove_ball(BallNode **list, int index);
void clear_balls(BallNode **list);
void add_random_ball(void);
void remove_last_ball(void);
void handle_wall_collision(Ball *b, Border *border_area, float restitution);
#endif
