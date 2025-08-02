#include "../include/input.h"
#include "../include/graphics.h"
#include "../include/objects.h"

extern State state;
extern int running;
extern Circle_Button *start_button;
extern Border *border;
extern ShowRender *show_render;
extern char text[20];

void process_initial_screen_input(void) {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      running = FALSE;
      break;

    case SDL_MOUSEBUTTONDOWN:
      if (event.button.button == SDL_BUTTON_LEFT) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        if (sqrt(pow(x - start_button->x, 2) + pow(y - start_button->y, 2)) <=
            start_button->radius) {
          state = RUNNING;
          *border = (Border){55, 55, (BOX_FACTOR_X * SCREEN_WIDTH) - 5,
                             (BOX_FACTOR_Y * SCREEN_HEIGHT) - 5};
          sprintf(text, "Bola: %d", 0);
          shuffle_balls();
        }
      }
      break;

    case SDL_KEYDOWN:
      switch (event.key.keysym.sym) {
      case SDLK_ESCAPE:
        running = FALSE;
        break;
      }
      break;
    }
  }
}

void process_input(void) {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      running = FALSE;
      break;

    case SDL_KEYDOWN:
      switch (event.key.keysym.sym) {
      case SDLK_ESCAPE:
        running = FALSE;
        break;

      case SDLK_p:
        if (state == RUNNING) {
          state = PAUSED;
        } else if (state == PAUSED) {
          state = RUNNING;
        }
        break;
      case SDLK_s:
        shuffle_balls();
        break;

      case SDLK_g:
        show_render->gravity = !show_render->gravity;
        break;

      case SDLK_c:
        show_render->mass_center = !show_render->mass_center;
        break;

      case SDLK_v:
        show_render->vectors = !show_render->vectors;
        break;

      case SDLK_m:
        show_render->mute = !show_render->mute;
        break;

      case SDLK_PLUS:
      case SDLK_KP_PLUS:
        add_random_ball();
        break;

      case SDLK_MINUS:
      case SDLK_KP_MINUS:
        remove_last_ball();
        break;
      }
      break;
    }
  }
}

