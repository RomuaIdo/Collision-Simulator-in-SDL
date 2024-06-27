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
    if((rect.x >= SCREEN_WIDTH - rect.width || rect.x <= 0) && collisionx == FALSE){
        xi = -xi*CR;
        collisionx = TRUE;
    }
    else{
        collisionx = FALSE;
    }
    if((rect.y >= SCREEN_HEIGHT - rect.height || rect.y <= 0) && collisiony == FALSE){
        yi = -yi*CR;
        collisiony = TRUE;
    }
    else
    {
        collisiony = FALSE;
    }

    rect.x += xi;
    rect.y += yi;

  
}