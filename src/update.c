#include <stdio.h>
#include <SDL2/SDL.h>
#include "./headers/globals.h"
#include "./headers/functions.h"
#include "./headers/structs.h"
int collisionx = FALSE;
int collisiony = FALSE;
void process_input(void){
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

void update(void){

    while(!SDL_TICKS_PASSED(SDL_GetTicks(), last_frame_time + FRAME_TARGET_TIME));
    
    float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;

    last_frame_time = SDL_GetTicks();

    

    if((rect.x >= SCREEN_WIDTH - rect.width || rect.x <= 0) && rect.collisionx == FALSE){
        rect.vx = -rect.vx*CR;
        rect.collisionx = TRUE;
    }
    else{
        rect.collisionx = FALSE;
    }
    if((rect.y >= SCREEN_HEIGHT - rect.height || rect.y <= 0) && collisiony == FALSE){
        rect.vy = -rect.vy*CR;
        rect.collisiony = TRUE;
    }
    else
    {
        rect.collisiony = FALSE;
    }

    rect.x += rect.vx * delta_time;
    rect.y += rect.vy * delta_time;
}