#include "./headers/macros.h"
#include "./headers/functions.h"
#include "./headers/structs.h"

// Variáveis Globais
int last_frame_time; // Tempo do último frame
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
int running = FALSE;

State state = PROCESSING;
Circle_Button *start_button;
Mix_Chunk *collision_sound;
float CR = 1.0;
Ball ball;
Ball **balls;
int n_balls = 4;
Collision ***collisions;
Triangle *triangle;
Border *border;
SDL_Rect *box;
int gravity = FALSE;
TTF_Font *font = NULL;
Texto *titulo;
SDL_Texture *text_texture = NULL;
SDL_Rect *text_rect;
char text[20] = "Collision Simulator";

int initialize(void)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
        return FALSE;
    }

    if (TTF_Init() != 0)
    {
        fprintf(stderr, "Error initializing SDL_ttf: %s\n", TTF_GetError());
        return FALSE;
    }
    
    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_BORDERLESS);
    if (!window)
    {
        fprintf(stderr, "Error creating SDL window: %s\n", SDL_GetError());
        return FALSE;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);

    if (!renderer)
    {
        fprintf(stderr, "Error creating SDL renderer: %s\n", SDL_GetError());
        return FALSE;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        fprintf(stderr, "Error initializing SDL_mixer: %s\n", Mix_GetError());
        return FALSE;
    }
    
    return TRUE;
}

Mix_Chunk *loadSound(const char *path)
{
    Mix_Chunk *sound = Mix_LoadWAV(path);
    if (sound == NULL)
    {
        printf("Erro ao carregar som: %s\n", Mix_GetError());
    }
    return sound;
}

void load_font(const char *font_path, int font_size)
{
    font = TTF_OpenFont(font_path, font_size);
    if (!font)
    {
        fprintf(stderr, "Error loading font: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
}

void setup_initial_screen(void)
{
    text_rect = (SDL_Rect *)malloc(sizeof(SDL_Rect));
    titulo = (Texto *)malloc(sizeof(Texto));
    titulo->color = (SDL_Color){255, 255, 255, 255};
    titulo->size = 48;
    load_font("./assets/stardew-valley.ttf", titulo->size); // Adjust the path and size as needed
    create_text_texture(renderer, text, titulo->color);
    border = (Border *)malloc(sizeof(Border));
    *border = (Border){0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    triangle = (Triangle *)malloc(sizeof(Triangle));
    start_button = (Circle_Button *)malloc(sizeof(Circle_Button));
    start_button->x = SCREEN_WIDTH / 2;
    start_button->y = SCREEN_HEIGHT / 2;
    start_button->radius = 50;
    start_button->r = 255;
    start_button->g = 255;
    start_button->b = 255;
    start_button->a = 255;
    triangle->points[0] = (SDL_Point){(start_button->x - (TRIANGLE_FACTOR) * (start_button->radius / 2)), (start_button->y - (TRIANGLE_FACTOR)*start_button->radius)};
    triangle->points[1] = (SDL_Point){(start_button->x - (TRIANGLE_FACTOR) * (start_button->radius / 2)), (start_button->y + (TRIANGLE_FACTOR)*start_button->radius)};
    triangle->points[2] = (SDL_Point){(start_button->x + (TRIANGLE_FACTOR) * (start_button->radius)), (start_button->y)};
    triangle->r = 0;
    triangle->g = 0;
    triangle->b = 0;
    triangle->a = 255;
}

void setup()
{
    srand(time(NULL));
    int i, j;
    // Mix_AllocateChannels(32);
    box = (SDL_Rect *)malloc(sizeof(SDL_Rect));
    *box = (SDL_Rect){50, 50, (BOX_FACTOR_X * SCREEN_WIDTH) - 50, (BOX_FACTOR_Y * SCREEN_HEIGHT) - 50};
    collision_sound = loadSound("./assets/collision.wav");
    collisions = (Collision ***)malloc(n_balls * sizeof(Collision **));
    for (i = 0; i < n_balls; i++)
    {
        collisions[i] = (Collision **)malloc(n_balls * sizeof(Collision *));
        for (j = 0; j < n_balls; j++)
        {
            collisions[i][j] = (Collision *)malloc(sizeof(Collision));
        }
    }

    balls = (Ball **)malloc(n_balls * sizeof(Ball *));
    for (i = 0; i < n_balls; i++)
    {
        balls[i] = (Ball *)malloc(sizeof(Ball)); // Aloca memória para cada Ball
        balls[i]->mass = (((double)rand() / RAND_MAX) * 2) + 1;
        balls[i]->radius = (int)(balls[i]->mass * 15);
        balls[i]->V = 400;
        balls[i]->angle = ((double)rand() / RAND_MAX) * M_PI;
        balls[i]->vx = balls[i]->V * cos(balls[i]->angle);
        balls[i]->vy = balls[i]->V * sin(balls[i]->angle);
        balls[i]->x = (double)rand() / RAND_MAX * (SCREEN_WIDTH - 2 * balls[i]->radius);
        balls[i]->y = (double)rand() / RAND_MAX * (SCREEN_HEIGHT - 2 * balls[i]->radius);
        balls[i]->collision_wallx = FALSE;
        balls[i]->collision_wally = FALSE;
        balls[i]->color_r = rand() % 256;
        balls[i]->color_g = rand() % 256;
        balls[i]->color_b = rand() % 256;
        balls[i]->color_a = 255;
    }
    last_frame_time = 0;
    state = INITIAL_SCREEN;
}
void destroy_window(void)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_FreeChunk(collision_sound);
    Mix_CloseAudio();
    SDL_Quit();
}

void free_alocatedmemory(void)
{
    int i, j;
    // Free colisions
    if (collisions != NULL)
    {
        for (i = 0; i < n_balls; i++)
        {
            for (j = 0; j < n_balls; j++)
            {
                free(collisions[i][j]); // Libera cada Collision individualmente
            }
            free(collisions[i]); // Depois libera o array de ponteiros
        }
        free(collisions); // Depois libera o array de ponteiros
    }

    if (balls != NULL)
    {
        for (i = 0; i < n_balls; i++)
        {
            free(balls[i]); // Libera cada Ball individualmente
        }
        free(balls); // Depois libera o array de ponteiros
    }
    free(text_rect);
    free(titulo);
    free(border);
    free(triangle);
    free(start_button);
    free(box);
}
