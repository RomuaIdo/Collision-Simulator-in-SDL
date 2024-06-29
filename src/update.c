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
    int i, j;
    float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;
    float distanceij;
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
            distanceij = sqrt(pow(balls[i]->x - balls[j]->x, 2) + pow(balls[i]->y - balls[j]->y, 2));
            if (distanceij <= balls[i]->radius + balls[j]->radius)
            {
                collisions[i][j]->collision = TRUE;
                collisions[i][j]->angle = atan2(balls[j]->y - balls[i]->y, balls[j]->x - balls[i]->x);
                collisions[i][j]->nx = cos(collisions[i][j]->angle);
                collisions[i][j]->ny = sin(collisions[i][j]->angle);
                collisions[i][j]->vcm_x = (balls[i]->mass*balls[i]->vx + balls[j]->mass*balls[j]->vx) / (balls[i]->mass + balls[j]->mass);
                collisions[i][j]->vcm_y = (balls[i]->mass*balls[i]->vy + balls[j]->mass*balls[j]->vy) / (balls[i]->mass + balls[j]->mass);
            }
            else
            {
                collisions[i][j]->collision = FALSE;
            }

            if (collisions[i][j]->collision == TRUE)
            {
                if(CR == 0){
                    // Colisão completamente inelástica
                    balls[i]->vx = balls[j]->vx = collisions[i][j]->vcm_x;
                    balls[i]->vy = balls[j]->vy = collisions[i][j]->vcm_y;

                    balls[i]->V = sqrt(pow(balls[i]->vx, 2) + pow(balls[i]->vy, 2));
                    balls[j]->V = sqrt(pow(balls[j]->vx, 2) + pow(balls[j]->vy, 2));

                    balls[i]->angle = atan2(balls[i]->vy, balls[i]->vx);
                    balls[j]->angle = atan2(balls[j]->vy, balls[j]->vx);

                    // Corrigir a sobreposição das bolas
                    float overlap = (balls[i]->radius + balls[j]->radius) - distanceij;
                    balls[i]->x -= overlap / 2 * collisions[i][j]->nx;
                    balls[i]->y -= overlap / 2 * collisions[i][j]->ny;
                    balls[j]->x += overlap / 2 * collisions[i][j]->nx;
                    balls[j]->y += overlap / 2 * collisions[i][j]->ny;
                }
                else{
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

                float overlap = (balls[i]->radius + balls[j]->radius) - distanceij;
                balls[i]->x -= overlap / 2 * collisions[i][j]->nx;
                balls[i]->y -= overlap / 2 * collisions[i][j]->ny;
                balls[j]->x += overlap / 2 * collisions[i][j]->nx;
                balls[j]->y += overlap / 2 * collisions[i][j]->ny;
                }
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
            if (CR == 0)
            {
                balls[i]->vy = 0;
                balls[i]->vx = 0;
            }
        }
        else
        {
            balls[i]->collision_wally = FALSE;
        }
        balls[i]->x += balls[i]->vx * delta_time;
        balls[i]->y += balls[i]->vy * delta_time;
    }
}