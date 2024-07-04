#include "./headers/macros.h"
#include "./headers/globals.h"
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
            if (sqrt(pow(x - start_button->x, 2) + pow(y - start_button->y, 2)) <= start_button->radius)
            {
                state = RUNNING;
                *border = (Border){55, 55, (BOX_FACTOR_X * SCREEN_WIDTH) - 5, (BOX_FACTOR_Y * SCREEN_HEIGHT) - 5};
                sprintf(text, "Bola: %d", 0);
                shuffle_balls();
            }
        }
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

        case SDLK_p:
            if (state == RUNNING)
            {
                state = PAUSED;
            }
            else if (state == PAUSED)
            {
                state = RUNNING;
            }
            break;
        case SDLK_s:
            shuffle_balls();
            break;

        case SDLK_g:
            if (show_render->gravity == TRUE)
            {
                show_render->gravity = FALSE;
            }
            else
            {
                show_render->gravity = TRUE;
            }
            break;

        case SDLK_c:
            if (show_render->mass_center == TRUE)
            {
                show_render->mass_center = FALSE;
            }
            else
            {
                show_render->mass_center = TRUE;
            }
            break;

        case SDLK_v:
            if (show_render->vectors == TRUE)
            {
                show_render->vectors = FALSE;
            }
            else
            {
                show_render->vectors = TRUE;
            }
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

    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME)
    {
        SDL_Delay(time_to_wait);
    }
    if (state != PAUSED)
    {
        update_positions(delta_time);
    }
}

void update_positions(float delta_time)
{
    int i, j;
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
                if (state == RUNNING)
                {
                    Mix_VolumeChunk(collision_sound, (int)collisions[i][j]->impulse_n);
                    Mix_PlayChannel(-1, collision_sound, 0);
                }
            }
        }
    }

    for (i = 0; i < n_balls; i++)
    {
        balls[i]->x = fmax(balls[i]->radius, fmin(balls[i]->x, border->x2 - balls[i]->radius));
        balls[i]->y = fmax(balls[i]->radius, fmin(balls[i]->y, border->y2 - balls[i]->radius));
        if (balls[i]->collision_wallx == FALSE && (balls[i]->x >= border->x2 - balls[i]->radius || balls[i]->x - balls[i]->radius <= border->x1))
        {
            balls[i]->x = fmax(balls[i]->radius + border->x1, fmin(balls[i]->x, border->x2 - balls[i]->radius));
            balls[i]->collision_wallx = TRUE;
            balls[i]->angle = M_PI - balls[i]->angle;
            balls[i]->vx = -(balls[i]->V * cos(balls[i]->angle)) * CR;
            balls[i]->V = sqrt(pow(balls[i]->vx, 2) + pow(balls[i]->vy, 2));
        }
        else
        {
            balls[i]->collision_wallx = FALSE;
        }

        if (balls[i]->collision_wally == FALSE && (balls[i]->y >= border->y2 - balls[i]->radius || balls[i]->y - balls[i]->radius <= border->y1))
        {
            balls[i]->y = fmax(balls[i]->radius + border->y1, fmin(balls[i]->y, border->y2 - balls[i]->radius));
            balls[i]->collision_wally = TRUE;
            balls[i]->angle = 2 * M_PI - balls[i]->angle;
            balls[i]->vy = -(balls[i]->V * sin(balls[i]->angle)) * CR;
            balls[i]->V = sqrt(pow(balls[i]->vx, 2) + pow(balls[i]->vy, 2));
        }
        else
        {
            balls[i]->collision_wally = FALSE;
        }
        if (show_render->gravity == TRUE)
        {
            balls[i]->vy += GRAVITY;
            balls[i]->y = fmax(0, balls[i]->y);
        }
        balls[i]->x += balls[i]->vx * delta_time;
        balls[i]->y += balls[i]->vy * delta_time;
    }
    if(show_render->mass_center == TRUE){
        update_mass_center();
    }
}
void update_mass_center(void)
{
    int i;
    float x = 0, y = 0, vx = 0, vy = 0, total_mass = 0;
    for (i = 0; i < n_balls; i++)
    {
        x += balls[i]->x * balls[i]->mass;
        y += balls[i]->y * balls[i]->mass;
        vx += balls[i]->vx * balls[i]->mass;
        vy += balls[i]->vy * balls[i]->mass;
        total_mass += balls[i]->mass;
    }
    mass_center->vx = vx / total_mass;
    mass_center->vy = vy / total_mass;
    mass_center->x = x / total_mass;
    mass_center->y = y / total_mass;
    mass_center->total_mass = total_mass;
}

void shuffle_balls(void)
{
    int i;
    for (i = 0; i < n_balls; i++)
    {
        balls[i]->x = (double)rand() / RAND_MAX * (border->x2 - 2 * balls[i]->radius) + (balls[i]->radius + border->x1);
        balls[i]->y = (double)rand() / RAND_MAX * (border->y2 - 2 * balls[i]->radius) + (balls[i]->radius + border->y1);
        balls[i]->V = 400;
        balls[i]->angle = ((double)rand() / RAND_MAX) * M_PI;
        balls[i]->vx = balls[i]->V * cos(balls[i]->angle);
        balls[i]->vy = balls[i]->V * sin(balls[i]->angle);
    }
}