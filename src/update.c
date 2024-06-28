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
    // Uncomment this block of code to limit the frame rate
    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);

    if(time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME){
        SDL_Delay(time_to_wait);
    }
    
    if (ball.collisionx == FALSE && (ball.x >= SCREEN_WIDTH - ball.radius || ball.x - ball.radius <= 0))
    {
        ball.x = fmax(ball.radius, fmin(ball.x, SCREEN_WIDTH - ball.radius));
        ball.collisionx = TRUE;
        ball.angle = M_PI-ball.angle;
        ball.vx = -(ball.V*cos(ball.angle)) * CR;
        ball.V = sqrt(pow(ball.vx, 2) + pow(ball.vy, 2));
    }
    else
    {
        ball.collisionx = FALSE;
    }

    if (ball.collisiony == FALSE && (ball.y >= SCREEN_HEIGHT - ball.radius || ball.y - ball.radius <= 0))
    {
        ball.y = fmax(ball.radius, fmin(ball.y, SCREEN_HEIGHT - ball.radius));
        ball.collisiony = TRUE;
        ball.angle = 2 * M_PI - ball.angle;
        ball.vy = -(ball.V*sin(ball.angle)) * CR;
        ball.V = sqrt(pow(ball.vx, 2) + pow(ball.vy, 2));
    }
    else
    {
        ball.collisiony = FALSE;
    }

    printf("vx: %f, vy: %f\n", ball.vx, ball.vy);
    ball.x += ball.vx * delta_time;
    ball.y += ball.vy * delta_time;

}