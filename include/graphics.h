#ifndef GRAPHICS_H
#define GRAPHICS_H
#include "utils.h"
#include <SDL2/SDL.h>

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 900
#define FPS 60
#define FRAME_TARGET_TIME (1000 / FPS)
#define TRIANGLE_FACTOR 0.8
#define BOX_FACTOR_X 0.75
#define BOX_FACTOR_Y 0.8

typedef struct Simulator Simulator;
typedef struct Resources Resources;

typedef struct {
  char text_buffer[20];
  SDL_Texture *text_texture;
  SDL_Rect* text_rect;
  SDL_Color color;
  int size;
} Text;

typedef struct Border {
  int x1, y1;
  int x2, y2;
} Border;

typedef struct {
  double x, y;
  double radius;
  unsigned char r, g, b, a;
} Circle_Button;

typedef struct {
  SDL_Point points[3];
  unsigned char r, g, b, a;
} Triangle;

typedef struct WindowConfig {
  SDL_Window *window;
  SDL_Renderer *renderer;
  int width;
  int height;
} WindowConfig;

void render(Simulator *simulator);
void fillTriangle(SDL_Renderer *renderer, SDL_Point *p);
void draw_box(SDL_Renderer *renderer, SDL_Rect *rect, int thickness);
void draw_arrow(SDL_Renderer *renderer, int x1, int y1, int x2, int y2);
int SDL_RenderFillCircle(SDL_Renderer *renderer, int x, int y, int radius);
int SDL_RenderDrawCircle(SDL_Renderer *renderer, int x, int y, int radius);
void load_font(Text* text, Resources* resources, const char *font_path);
void create_text_texture(Text* text, Resources* resources, WindowConfig* window_config);
void destroy_window(WindowConfig* window_config, Resources* resources, Text* text);

#endif
