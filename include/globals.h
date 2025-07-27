#ifndef GLOBALS_H
#define GLOBALS_H
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include "./structs.h"

extern int last_frame_time;
extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern int running;
extern float CR;
extern int n_balls;
extern State state;
extern Circle_Button *start_button;
extern Mix_Chunk *collision_sound;
extern Triangle *triangle;
extern BallNode *balls;
extern Border *border;
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
