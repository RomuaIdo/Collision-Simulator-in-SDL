#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>
#include "./headers/globals.h"
#include "./headers/functions.h"
#include "./headers/structs.h"


//Variáveis Globais
int last_frame_time; //Tempo do último frame
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
int running = FALSE;

float CR = 1.01;
Ball ball;


int initialize_window(void){
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
        fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
        return FALSE;
    }
    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_BORDERLESS
    );
    if(!window){
        fprintf(stderr, "Error creating SDL window: %s\n", SDL_GetError());
        return FALSE;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);

    if(!renderer){
        fprintf(stderr, "Error creating SDL renderer: %s\n", SDL_GetError());
        return FALSE;
    }
    
    return TRUE;
}

void setup(){
    last_frame_time = 0;
    ball.V = 100;
    ball.angle = 3.0*M_PI/4.0;
    ball.vx = ball.V * cos(ball.angle);
    ball.vy = ball.V * sin(ball.angle);
    ball.x = 55;
    ball.y = 55;
    ball.collisionx = FALSE;
    ball.collisiony = FALSE;
    ball.radius = 25;
}
void destroy_window(void){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
