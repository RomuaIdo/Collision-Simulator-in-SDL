#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>
#include <time.h>
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
Ball **balls;
int n_balls = 3;


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
    srand(time(NULL));
    int i;
    printf("Setup done\n");
    balls = (Ball**)malloc(n_balls*sizeof(Ball*));
    for(i = 0; i < n_balls; i++){
        balls[i] = (Ball*)malloc(sizeof(Ball)); // Aloca memória para cada Ball
        balls[i]->V = 100;
        balls[i]->angle = ((double)rand()/RAND_MAX)*M_PI;
        balls[i]->vx = balls[i]->V * cos(balls[i]->angle);
        balls[i]->vy = balls[i]->V * sin(balls[i]->angle);
        balls[i]->x = 55;
        balls[i]->y = 55;
        balls[i]->collision_wallx = FALSE;
        balls[i]->collision_wally = FALSE;
        balls[i]->radius = 25;
    }
    

    last_frame_time = 0;
}
void destroy_window(void){
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
