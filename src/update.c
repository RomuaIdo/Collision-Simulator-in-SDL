#include <stdio.h>
#include <SDL2/SDL.h>
#include "./headers/globals.h"
#include "./headers/macros.h"
#include "./headers/functions.h"
#include "./headers/structs.h"
int collision_wallx = FALSE;
int collision_wally = FALSE;

void process_initial_screen_input(void)
{
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type)
    {
    case SDL_QUIT:
        running = FALSE;
        break;

    case SDL_MOUSEBUTTONDOWN:
        if (event.button.button == SDL_BUTTON_LEFT)
        {
            int x, y;
            SDL_GetMouseState(&x, &y);
            if (sqrt(pow(x - start_button.x, 2) + pow(y - start_button.y, 2)) <= start_button.radius)
            {
                state = PROCESSING;
            }
        }
        break;
    }
}

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
    int i, j;
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
        collisions[i][i]->collision = FALSE;

        for (j = i + 1; j < n_balls; j++)
        {
            collisions[i][j]->distance = sqrt(pow(balls[i]->x - balls[j]->x, 2) + pow(balls[i]->y - balls[j]->y, 2));
            if (collisions[i][j]->distance <= balls[i]->radius + balls[j]->radius)
            {
                collisions[i][j]->collision = TRUE;
                collisions[i][j]->angle = atan2(balls[j]->y - balls[i]->y, balls[j]->x - balls[i]->x);
                collisions[i][j]->nx = cos(collisions[i][j]->angle);
                collisions[i][j]->ny = sin(collisions[i][j]->angle);
                collisions[i][j]->vcm_x = (balls[i]->mass * balls[i]->vx + balls[j]->mass * balls[j]->vx) / (balls[i]->mass + balls[j]->mass);
                collisions[i][j]->vcm_y = (balls[i]->mass * balls[i]->vy + balls[j]->mass * balls[j]->vy) / (balls[i]->mass + balls[j]->mass);

                // Corrigir a sobreposição das bolas caso a colisao nao seja elastica

                collisions[i][j]->overlap = (balls[i]->radius + balls[j]->radius) - collisions[i][j]->distance;
                balls[i]->x -= collisions[i][j]->overlap * collisions[i][j]->nx;
                balls[i]->y -= collisions[i][j]->overlap * collisions[i][j]->ny;
                balls[j]->x += collisions[i][j]->overlap * collisions[i][j]->nx;
                balls[j]->y += collisions[i][j]->overlap * collisions[i][j]->ny;

            }
            else
            {
                collisions[i][j]->collision = FALSE;
            }

            if (collisions[i][j]->collision == TRUE)
            {
                Mix_PlayChannel(-1, collision_sound, 0);
                collisions[i][j]->vrel_n = (balls[j]->vx - balls[i]->vx) * collisions[i][j]->nx + (balls[j]->vy - balls[i]->vy) * collisions[i][j]->ny;

                collisions[i][j]->impulse_n = (-(1 + CR) * collisions[i][j]->vrel_n) / (1 / balls[i]->mass + 1 / balls[j]->mass);

                balls[i]->vx -= collisions[i][j]->impulse_n * collisions[i][j]->nx / balls[i]->mass;
                balls[i]->vy -= collisions[i][j]->impulse_n * collisions[i][j]->ny / balls[i]->mass;
                balls[i]->V = sqrt(pow(balls[i]->vx, 2) + pow(balls[i]->vy, 2));
                balls[i]->angle = atan2(balls[i]->vy, balls[i]->vx);

                balls[j]->vx += collisions[i][j]->impulse_n * collisions[i][j]->nx / balls[j]->mass;
                balls[j]->vy += collisions[i][j]->impulse_n * collisions[i][j]->ny / balls[j]->mass;
                balls[j]->V = sqrt(pow(balls[j]->vx, 2) + pow(balls[j]->vy, 2));
                balls[j]->angle = atan2(balls[j]->vy, balls[j]->vx);
            }
        }
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
            if (CR == 0)
            {
                balls[i]->vx = 0;
                balls[i]->vy = 0;
            }
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
        balls[i]->x = fmax(balls[i]->radius, fmin(balls[i]->x, SCREEN_WIDTH - balls[i]->radius));
        balls[i]->y = fmax(balls[i]->radius, fmin(balls[i]->y, SCREEN_HEIGHT - balls[i]->radius));
        balls[i]->x += balls[i]->vx * delta_time;
        balls[i]->y += balls[i]->vy * delta_time;
    }
}