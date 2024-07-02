#include "./headers/globals.h"
#include "./headers/macros.h"
#include "./headers/functions.h"
#include "./headers/structs.h"

int SDL_RenderDrawCircle(SDL_Renderer * renderer, int x, int y, int radius)
{
    int offsetx, offsety, d;
    int status;


    offsetx = 0;
    offsety = radius;
    d = radius -1;
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

        if (d >= 2*offsetx) {
            d -= 2*offsetx + 1;
            offsetx +=1;
        }
        else if (d < 2 * (radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }

    return status;
}

int SDL_RenderFillCircle(SDL_Renderer * renderer, int x, int y, int radius)
{
    int offsetx, offsety, d;
    int status;


    offsetx = 0;
    offsety = radius;
    d = radius -1;
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

        if (d >= 2*offsetx) {
            d -= 2*offsetx + 1;
            offsetx +=1;
        }
        else if (d < 2 * (radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }

    return status;
}

void fillTriangle(SDL_Renderer* renderer, SDL_Point* p) {
    // Ordena os pontos por y (p[0].y <= p[1].y <= p[2].y)
    if (p[1].y < p[0].y) { SDL_Point temp = p[0]; p[0] = p[1]; p[1] = temp; }
    if (p[2].y < p[0].y) { SDL_Point temp = p[0]; p[0] = p[2]; p[2] = temp; }
    if (p[2].y < p[1].y) { SDL_Point temp = p[1]; p[1] = p[2]; p[2] = temp; }

    int total_height = p[2].y - p[0].y;

    for (int i = 0; i < total_height; i++) {
        int second_half = i > p[1].y - p[0].y || p[1].y == p[0].y;  // int para bool
        int segment_height = second_half ? p[2].y - p[1].y : p[1].y - p[0].y;
        float alpha = (float)i / total_height;
        float beta  = (float)(i - (second_half ? p[1].y - p[0].y : 0)) / segment_height;

        SDL_Point A = { p[0].x + (p[2].x - p[0].x) * alpha, p[0].y + i };
        SDL_Point B = second_half ? 
            (SDL_Point){ p[1].x + (p[2].x - p[1].x) * beta, p[1].y + (i - (p[1].y - p[0].y)) } :
            (SDL_Point){ p[0].x + (p[1].x - p[0].x) * beta, p[0].y + i };

        if (A.x > B.x) { SDL_Point temp = A; A = B; B = temp; }

        for (int j = A.x; j <= B.x; j++) {
            SDL_RenderDrawPoint(renderer, j, p[0].y + i);
        }
    }
}



void render_initial_screen(void)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Renderiza o botão de início
    SDL_SetRenderDrawColor(renderer, start_button->r, start_button->g, start_button->b, start_button->a);
    SDL_RenderFillCircle(renderer, start_button->x, start_button->y, start_button->radius);

    // Renderiza o triângulo no botão de início
    SDL_SetRenderDrawColor(renderer, triangle->r, triangle->g, triangle->b, triangle->a);
    fillTriangle(renderer, triangle->points);

    SDL_RenderPresent(renderer);
}


void render(void)
{
    int i;
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    for(i = 0; i < n_balls; i++){
        SDL_SetRenderDrawColor(renderer, balls[i]->color_r, balls[i]->color_g, balls[i]->color_b, balls[i]->color_a);
        SDL_RenderFillCircle(renderer, balls[i]->x, balls[i]->y, balls[i]->radius);
    }

    SDL_RenderPresent(renderer);
}