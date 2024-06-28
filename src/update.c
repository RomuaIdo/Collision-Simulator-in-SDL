#include <stdio.h>
#include <SDL2/SDL.h>
#include "./headers/globals.h"
#include "./headers/functions.h"
#include "./headers/structs.h"
int collision_wallx = FALSE;
int collision_wally = FALSE;
void process_input(void)
{
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type)
    {
    case SDL_QUIT:
        running = FALSE;
        break;

    case SDL_KEYDOWN:
        switch (event.key.keysym.sym)
        {
        case SDLK_ESCAPE:
            running = FALSE;
            break;
        }
        break;
    }
}

void update(void)
{
    int i;
    float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;

    last_frame_time = SDL_GetTicks();
    // Uncomment this block of code to limit the frame rate
    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);

    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
    {
        SDL_Delay(time_to_wait);
    }

    for (i = 0; i < n_balls; i++)
    {
        if (balls[i]->collision_wallx == FALSE && (balls[i]->x >= SCREEN_WIDTH - balls[i]->radius || balls[i]->x - balls[i]->radius <= 0))
        {
            balls[i]->x = fmax(balls[i]->radius, fmin(balls[i]->x, SCREEN_WIDTH - balls[i]->radius));
            balls[i]->collision_wallx = TRUE;
            balls[i]->angle = M_PI - balls[i]->angle;
            balls[i]->vx = -(balls[i]->V * cos(balls[i]->angle)) * CR;
            balls[i]->V = sqrt(pow(balls[i]->vx, 2) + pow(balls[i]->vy, 2));
        }
        else
        {
            balls[i]->collision_wallx = FALSE;
        }

        if (balls[i]->collision_wally == FALSE && (balls[i]->y >= SCREEN_HEIGHT - balls[i]->radius || balls[i]->y - balls[i]->radius <= 0))
        {
            balls[i]->y = fmax(balls[i]->radius, fmin(balls[i]->y, SCREEN_HEIGHT - balls[i]->radius));
            balls[i]->collision_wally = TRUE;
            balls[i]->angle = 2 * M_PI - balls[i]->angle;
            balls[i]->vy = -(balls[i]->V * sin(balls[i]->angle)) * CR;
            balls[i]->V = sqrt(pow(balls[i]->vx, 2) + pow(balls[i]->vy, 2));
        }
        else
        {
            balls[i]->collision_wally = FALSE;
        }
        balls[i]->x += balls[i]->vx * delta_time;
        balls[i]->y += balls[i]->vy * delta_time;
    }
}