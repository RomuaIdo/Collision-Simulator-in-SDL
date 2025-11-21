#include "../include/graphics.h"
#include "../include/input.h"
#include "../include/main.h"
#include "../include/objects.h"

void draw_box(SDL_Renderer *renderer, SDL_Rect *rect, int thickness) {
  SDL_Rect top = {rect->x, rect->y, rect->w, thickness};
  SDL_RenderFillRect(renderer, &top);

  SDL_Rect bottom = {rect->x, rect->y + rect->h - thickness, rect->w,
                     thickness};
  SDL_RenderFillRect(renderer, &bottom);

  SDL_Rect left = {rect->x, rect->y, thickness, rect->h};
  SDL_RenderFillRect(renderer, &left);

  SDL_Rect right = {rect->x + rect->w - thickness, rect->y, thickness, rect->h};
  SDL_RenderFillRect(renderer, &right);
}

int SDL_RenderDrawCircle(SDL_Renderer *renderer, int x, int y, int radius) {
  int offsetx, offsety, d;
  int status;

  offsetx = 0;
  offsety = radius;
  d = radius - 1;
  status = 0;

  while (offsety >= offsetx) {
    status += SDL_RenderDrawPoint(renderer, x + offsetx, y + offsety);
    status += SDL_RenderDrawPoint(renderer, x + offsety, y + offsetx);
    status += SDL_RenderDrawPoint(renderer, x - offsetx, y + offsety);
    status += SDL_RenderDrawPoint(renderer, x - offsety, y + offsetx);
    status += SDL_RenderDrawPoint(renderer, x + offsetx, y - offsety);
    status += SDL_RenderDrawPoint(renderer, x + offsety, y - offsetx);
    status += SDL_RenderDrawPoint(renderer, x - offsetx, y - offsety);
    status += SDL_RenderDrawPoint(renderer, x - offsety, y - offsetx);

    if (status < 0) {
      status = -1;
      break;
    }

    if (d >= 2 * offsetx) {
      d -= 2 * offsetx + 1;
      offsetx += 1;
    } else if (d < 2 * (radius - offsety)) {
      d += 2 * offsety - 1;
      offsety -= 1;
    } else {
      d += 2 * (offsety - offsetx - 1);
      offsety -= 1;
      offsetx += 1;
    }
  }

  return status;
}

int SDL_RenderFillCircle(SDL_Renderer *renderer, int x, int y, int radius) {
  int offsetx, offsety, d;
  int status;

  offsetx = 0;
  offsety = radius;
  d = radius - 1;
  status = 0;

  while (offsety >= offsetx) {

    status += SDL_RenderDrawLine(renderer, x - offsety, y + offsetx,
                                 x + offsety, y + offsetx);
    status += SDL_RenderDrawLine(renderer, x - offsetx, y + offsety,
                                 x + offsetx, y + offsety);
    status += SDL_RenderDrawLine(renderer, x - offsetx, y - offsety,
                                 x + offsetx, y - offsety);
    status += SDL_RenderDrawLine(renderer, x - offsety, y - offsetx,
                                 x + offsety, y - offsetx);

    if (status < 0) {
      status = -1;
      break;
    }

    if (d >= 2 * offsetx) {
      d -= 2 * offsetx + 1;
      offsetx += 1;
    } else if (d < 2 * (radius - offsety)) {
      d += 2 * offsety - 1;
      offsety -= 1;
    } else {
      d += 2 * (offsety - offsetx - 1);
      offsety -= 1;
      offsetx += 1;
    }
  }

  return status;
}

void fillTriangle(SDL_Renderer *renderer, SDL_Point *p) {
  if (p[1].y < p[0].y) {
    SDL_Point temp = p[0];
    p[0] = p[1];
    p[1] = temp;
  }
  if (p[2].y < p[0].y) {
    SDL_Point temp = p[0];
    p[0] = p[2];
    p[2] = temp;
  }
  if (p[2].y < p[1].y) {
    SDL_Point temp = p[1];
    p[1] = p[2];
    p[2] = temp;
  }

  int total_height = p[2].y - p[0].y;

  for (int i = 0; i < total_height; i++) {
    int second_half = i > p[1].y - p[0].y || p[1].y == p[0].y; // int para bool
    int segment_height = second_half ? p[2].y - p[1].y : p[1].y - p[0].y;
    float alpha = (float)i / total_height;
    float beta =
        (float)(i - (second_half ? p[1].y - p[0].y : 0)) / segment_height;

    SDL_Point A = {p[0].x + (p[2].x - p[0].x) * alpha, p[0].y + i};
    SDL_Point B = second_half ? (SDL_Point){p[1].x + (p[2].x - p[1].x) * beta,
                                            p[1].y + (i - (p[1].y - p[0].y))}
                              : (SDL_Point){p[0].x + (p[1].x - p[0].x) * beta,
                                            p[0].y + i};

    if (A.x > B.x) {
      SDL_Point temp = A;
      A = B;
      B = temp;
    }

    for (int j = A.x; j <= B.x; j++) {
      SDL_RenderDrawPoint(renderer, j, p[0].y + i);
    }
  }
}

void draw_arrow(SDL_Renderer *renderer, int x1, int y1, int x2, int y2) {
  SDL_RenderDrawLine(renderer, x1, y1, x2, y2);

  float angle = atan2(y2 - y1, x2 - x1);

  float arrow_head_length = 10.0f;
  float arrow_head_angle = M_PI / 6; // 30 graus

  int arrow_x1 = x2 - (int)(arrow_head_length * cos(angle - arrow_head_angle));
  int arrow_y1 = y2 - (int)(arrow_head_length * sin(angle - arrow_head_angle));
  int arrow_x2 = x2 - (int)(arrow_head_length * cos(angle + arrow_head_angle));
  int arrow_y2 = y2 - (int)(arrow_head_length * sin(angle + arrow_head_angle));

  SDL_RenderDrawLine(renderer, x2, y2, arrow_x1, arrow_y1);
  SDL_RenderDrawLine(renderer, x2, y2, arrow_x2, arrow_y2);
}

void create_text_texture(Simulator *simulator, const char *text,
                         SDL_Color color) {
  SDL_Surface *text_surface =
      TTF_RenderText_Solid(simulator->font, text, color);
  if (!text_surface) {
    fprintf(stderr, "Error creating text surface: %s\n", TTF_GetError());
    exit(EXIT_FAILURE);
  }

  if (simulator->text_texture) {
    SDL_DestroyTexture(simulator->text_texture);
  }

  simulator->text_texture =
      SDL_CreateTextureFromSurface(simulator->renderer, text_surface);
  if (!simulator->text_texture) {
    fprintf(stderr, "Error creating text texture: %s\n", SDL_GetError());
    SDL_FreeSurface(text_surface);
    exit(EXIT_FAILURE);
  }

  simulator->text_rect->x = (SCREEN_WIDTH - text_surface->w) / 2;
  simulator->text_rect->y = (SCREEN_HEIGHT - text_surface->h) / 2 - 200;
  simulator->text_rect->w = text_surface->w;
  simulator->text_rect->h = text_surface->h;

  SDL_FreeSurface(text_surface);
}

void render(Simulator *simulator) {
  SDL_SetRenderDrawColor(simulator->renderer, 0, 0, 0, 255);
  SDL_RenderClear(simulator->renderer);

  for (int i = 0; i < simulator->ball_array.count; i++) {
    Ball *ball = &simulator->ball_array.data[i];
    SDL_SetRenderDrawColor(simulator->renderer, ball->color_r, ball->color_g,
                           ball->color_b, ball->color_a);
    SDL_RenderFillCircle(simulator->renderer, ball->x, ball->y, ball->radius);
  }

  if (simulator->state == INITIAL_SCREEN) {
    SDL_SetRenderDrawColor(simulator->renderer, simulator->start_button->r,
                           simulator->start_button->g,
                           simulator->start_button->b,
                           simulator->start_button->a);
    SDL_RenderFillCircle(simulator->renderer, simulator->start_button->x,
                         simulator->start_button->y,
                         simulator->start_button->radius);

    SDL_SetRenderDrawColor(simulator->renderer, simulator->triangle->r,
                           simulator->triangle->g, simulator->triangle->b,
                           simulator->triangle->a);
    fillTriangle(simulator->renderer, simulator->triangle->points);

    SDL_RenderCopy(simulator->renderer, simulator->text_texture, NULL,
                   simulator->text_rect);
  } else {
    SDL_SetRenderDrawColor(simulator->renderer, 255, 255, 255, 255);
    draw_box(simulator->renderer, simulator->box, 5);
    if (simulator->settings->vectors) {
      for (int i = 0; i < simulator->ball_array.count; i++) {
        Ball *ball = &simulator->ball_array.data[i];
        SDL_SetRenderDrawColor(simulator->renderer, ball->color_r,
                               ball->color_g, ball->color_b, ball->color_a);
        draw_arrow(simulator->renderer, ball->x, ball->y,
                   ball->x + (ball->vx / 2), ball->y + (ball->vy / 2));
      }
    }

    if (simulator->settings->mass_center) {
      SDL_SetRenderDrawColor(simulator->renderer, 255, 0, 0, 255);
      SDL_RenderFillCircle(simulator->renderer, simulator->mass_center->x,
                           simulator->mass_center->y,
                           simulator->mass_center->radius);

      if (simulator->settings->vectors) {
        SDL_SetRenderDrawColor(simulator->renderer, 255, 0, 0, 255);
        draw_arrow(simulator->renderer, simulator->mass_center->x,
                   simulator->mass_center->y,
                   simulator->mass_center->x + (simulator->mass_center->vx / 2),
                   simulator->mass_center->y +
                       (simulator->mass_center->vy / 2));
      }
    }
  }

  SDL_RenderPresent(simulator->renderer);
}

void load_font(Simulator *simulator, const char *font_path, int font_size) {
  if (simulator->font) {
    TTF_CloseFont(simulator->font);
  }

  simulator->font = TTF_OpenFont(font_path, font_size);
  if (!simulator->font) {
    fprintf(stderr, "Error loading font: %s\n", TTF_GetError());
    exit(EXIT_FAILURE);
  }
}

void destroy_window(Simulator *simulator) {
  free_alocatedmemory(simulator);

  if (simulator->renderer) {
    SDL_DestroyRenderer(simulator->renderer);
    simulator->renderer = NULL;
  }

  if (simulator->window) {
    SDL_DestroyWindow(simulator->window);
    simulator->window = NULL;
  }

  if (simulator->collision_sound) {
    Mix_FreeChunk(simulator->collision_sound);
    simulator->collision_sound = NULL;
  }

  if (simulator->font) {
    TTF_CloseFont(simulator->font);
    simulator->font = NULL;
  }

  if (simulator->text_texture) {
    SDL_DestroyTexture(simulator->text_texture);
    simulator->text_texture = NULL;
  }

  TTF_Quit();
  Mix_CloseAudio();
  SDL_Quit();
}
