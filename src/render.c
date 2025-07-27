#include "../include/functions.h"
#include "../include/globals.h"
#include "../include/macros.h"
#include "../include/structs.h"

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

void create_text_texture(SDL_Renderer *renderer, const char *text,
                         SDL_Color color) {
  SDL_Surface *text_surface = TTF_RenderText_Solid(font, text, color);
  if (!text_surface) {
    fprintf(stderr, "Error creating text surface: %s\n", TTF_GetError());
    exit(EXIT_FAILURE);
  }

  text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
  if (!text_texture) {
    fprintf(stderr, "Error creating text texture: %s\n", SDL_GetError());
    SDL_FreeSurface(text_surface);
    exit(EXIT_FAILURE);
  }

  text_rect->x = (SCREEN_WIDTH - text_surface->w) / 2;
  text_rect->y = (SCREEN_HEIGHT - text_surface->h) / 2 - 200;
  text_rect->w = text_surface->w;
  text_rect->h = text_surface->h;

  SDL_FreeSurface(text_surface);
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

void render(void) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  BallNode *current = balls;
  while (current) {
    Ball *ball = &(current->ball);
    SDL_SetRenderDrawColor(renderer, ball->color_r, ball->color_g,
                           ball->color_b, ball->color_a);
    SDL_RenderFillCircle(renderer, ball->x, ball->y, ball->radius);
    current = current->next;
  }

  if (state == INITIAL_SCREEN) {
    SDL_SetRenderDrawColor(renderer, start_button->r, start_button->g,
                           start_button->b, start_button->a);
    SDL_RenderFillCircle(renderer, start_button->x, start_button->y,
                         start_button->radius);

    SDL_SetRenderDrawColor(renderer, triangle->r, triangle->g, triangle->b,
                           triangle->a);
    fillTriangle(renderer, triangle->points);

    SDL_RenderCopy(renderer, text_texture, NULL, text_rect);
  } else {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    draw_box(renderer, box, 5);

    current = balls;
    while (current) {
      Ball *ball = &(current->ball);
      if (show_render->vectors) {
        SDL_SetRenderDrawColor(renderer, ball->color_r, ball->color_g,
                               ball->color_b, ball->color_a);
        draw_arrow(renderer, ball->x, ball->y, ball->x + (ball->vx / 2),
                   ball->y + (ball->vy / 2));
      }
      current = current->next;
    }

    if (show_render->mass_center) {
      SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
      SDL_RenderFillCircle(renderer, mass_center->x, mass_center->y,
                           mass_center->radius);

      if (show_render->vectors) {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        draw_arrow(renderer, mass_center->x, mass_center->y,
                   mass_center->x + (mass_center->vx / 2),
                   mass_center->y + (mass_center->vy / 2));
      }
    }
  }

  SDL_RenderPresent(renderer);
}
