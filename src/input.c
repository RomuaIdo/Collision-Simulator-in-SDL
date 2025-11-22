#include "../include/input.h"
#include "../include/graphics.h"
#include "../include/main.h"
#include "../include/objects.h"

void process_initial_screen_input(MainMenu* menu, PhysicsWorld* world, State* state, int* running) {
  SDL_Event event;
  Circle_Button *start_button = menu->start_button;
  Border *border = world->border;

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      (*running) = FALSE;
      break;

    case SDL_MOUSEBUTTONDOWN:
      if (event.button.button == SDL_BUTTON_LEFT) {
        int x, y;
        SDL_GetMouseState(&x, &y);
        if (sqrt(pow(x - start_button->x, 2) + pow(y - start_button->y, 2)) <=
            start_button->radius) {
          (*state) = RUNNING;
          *border = (Border){55, 55, (BOX_FACTOR_X * SCREEN_WIDTH) - 5,
                             (BOX_FACTOR_Y * SCREEN_HEIGHT) - 5};
          shuffle_balls(&world->ball_array, world->mass_center, border);
        }
      }
      break;

    case SDL_KEYDOWN:
      switch (event.key.keysym.sym) {
      case SDLK_ESCAPE:
        (*running) = FALSE;
        break;
      }
      break;
    }
  }
}

void process_input(Settings* settings, PhysicsWorld* world, State* state, int* running) {
  SDL_Event event;

  while (SDL_PollEvent(&event)) {
    switch (event.type) {
    case SDL_QUIT:
      (*running) = FALSE;
      break;

    case SDL_KEYDOWN:
      switch (event.key.keysym.sym) {
      case SDLK_ESCAPE:
        (*running) = FALSE;
        break;

      case SDLK_p:
        if ((*state) == RUNNING) {
          (*state) = PAUSED;
        } else if ((*state) == PAUSED) {
          (*state) = RUNNING;
        }
        break;
      case SDLK_s:
        shuffle_balls(&world->ball_array, world->mass_center,
                      world->border);
        break;

      case SDLK_g:
        settings->gravity = !settings->gravity;
        break;

      case SDLK_c:
        settings->mass_center = !settings->mass_center;
        break;

      case SDLK_v:
        settings->vectors = !settings->vectors;
        break;

      case SDLK_m:
        settings->mute = !settings->mute;
        break;

      case SDLK_PLUS:
      case SDLK_KP_PLUS:
        add_random_ball(&world->ball_array, world->border);
        break;

      case SDLK_MINUS:
      case SDLK_KP_MINUS:
        remove_last_ball(&world->ball_array, world->mass_center);
        break;
      }
      break;
    }
  }
}
