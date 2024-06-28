#include <stdio.h>
#include <SDL2/SDL.h>
#include "./headers/globals.h"
#include "./headers/functions.h"
#include "./headers/structs.h"
int collisionx = FALSE;
int collisiony = FALSE;
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

    float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;

    last_frame_time = SDL_GetTicks();
    /* Uncomment this block of code to limit the frame rate
    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);

    if(time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME){
        SDL_Delay(time_to_wait);
    }
    */
    if ((rect.x >= SCREEN_WIDTH - rect.radius || rect.x - rect.radius <= 0))
    {
        if (rect.collisionx == FALSE)
        {
            rect.collisionx = TRUE;
            rect.vx = -rect.vx * CR;
        }
    }
    else
    {
        rect.collisionx = FALSE;
    }

    if ((rect.y >= SCREEN_HEIGHT - rect.radius || rect.y - rect.radius <= 0))
    {
        if (rect.collisiony == FALSE)
        {
            rect.collisiony = TRUE;
            rect.vy = -rect.vy * CR;
        }
    }
    else
    {
        rect.collisiony = FALSE;
    }

    rect.x += rect.vx * delta_time;
    rect.y += rect.vy * delta_time;
}