#include "../include/main.h"
#include "../include/input.h"
#include "../include/objects.h"

int main(int argc, char *argv[]) {
  Simulator *simulator;
  simulator = (Simulator *)malloc(sizeof(Simulator));
  strcpy(simulator->text, "Collision Simulator");
  if (!simulator) {
    fprintf(stderr, "Memory allocation failed\n");
    return EXIT_FAILURE;
  }

  printf("Digite o coeficiente de restituição: ");
  scanf("%f", &simulator->CR);

  simulator->running = initialize(simulator);
  setup(simulator);
  setup_initial_screen(simulator);
  while (simulator->running) {
    if (simulator->state == INITIAL_SCREEN) {
      process_initial_screen_input(simulator);
      update(simulator);
      render(simulator);
    } else if (simulator->state == RUNNING || simulator->state == PAUSED) {
      process_input(simulator);
      update(simulator);
      render(simulator);
    }
  }
  destroy_window(simulator);

  return 0;
}

int initialize(Simulator *simulator) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
    return FALSE;
  }

  if (TTF_Init() != 0) {
    fprintf(stderr, "Error initializing SDL_ttf: %s\n", TTF_GetError());
    return FALSE;
  }

  simulator->window =
      SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_BORDERLESS);
  if (!simulator->window) {
    fprintf(stderr, "Error creating SDL window: %s\n", SDL_GetError());
    return FALSE;
  }

  simulator->renderer = SDL_CreateRenderer(simulator->window, -1, 0);

  if (!simulator->renderer) {
    fprintf(stderr, "Error creating SDL renderer: %s\n", SDL_GetError());
    return FALSE;
  }

  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
    fprintf(stderr, "Error initializing SDL_mixer: %s\n", Mix_GetError());
    return FALSE;
  }
  simulator->running = TRUE;
  return TRUE;
}

void setup(Simulator *simulator) {
  srand(time(NULL));
  // Mix_AllocateChannels(32);
  simulator->settings = (Settings *)malloc(sizeof(Settings));
  *(simulator->settings) = (Settings){FALSE, FALSE, FALSE, FALSE};
  simulator->mass_center = (MassCenter *)malloc(sizeof(MassCenter));
  *(simulator->mass_center) = (MassCenter){0, 0, 0, 0, 0, 5};
  simulator->box = (SDL_Rect *)malloc(sizeof(SDL_Rect));
  *(simulator->box) = (SDL_Rect){50, 50, (BOX_FACTOR_X * SCREEN_WIDTH) - 50,
                                 (BOX_FACTOR_Y * SCREEN_HEIGHT) - 50};


  init_collision_array(&simulator->collision_array, 128);
  simulator->collision_sound = loadSound("./assets/collision.wav");

  simulator->balls = NULL;
  for (int i = 0; i < 4; i++) {
    Ball *ball = generate_random_ball();
    add_ball(simulator, ball);
  }

  simulator->last_frame_time = 0;
  simulator->state = INITIAL_SCREEN;
}

void setup_initial_screen(Simulator *simulator) {
  simulator->text_rect = (SDL_Rect *)malloc(sizeof(SDL_Rect));
  simulator->titulo = (Texto *)malloc(sizeof(Texto));
  simulator->titulo->color = (SDL_Color){255, 255, 255, 255};
  simulator->titulo->size = 48;
  load_font(simulator, "./assets/stardew-valley.ttf", simulator->titulo->size);
  create_text_texture(simulator, simulator->text, simulator->titulo->color);
  simulator->border = (Border *)malloc(sizeof(Border));
  *(simulator->border) = (Border){0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
  simulator->triangle = (Triangle *)malloc(sizeof(Triangle));
  simulator->start_button = (Circle_Button *)malloc(sizeof(Circle_Button));
  simulator->start_button->x = (float)(SCREEN_WIDTH / 2.f);
  simulator->start_button->y = (float)(SCREEN_HEIGHT / 2.f);
  simulator->start_button->radius = 50;
  simulator->start_button->r = 255;
  simulator->start_button->g = 255;
  simulator->start_button->b = 255;
  simulator->start_button->a = 255;

  Circle_Button *start_button = simulator->start_button;
  Triangle *triangle = simulator->triangle;

  triangle->points[0] = (SDL_Point){
      (start_button->x - (TRIANGLE_FACTOR) * (start_button->radius / 2)),
      (start_button->y - (TRIANGLE_FACTOR)*start_button->radius)};
  triangle->points[1] = (SDL_Point){
      (start_button->x - (TRIANGLE_FACTOR) * (start_button->radius / 2)),
      (start_button->y + (TRIANGLE_FACTOR)*start_button->radius)};
  triangle->points[2] = (SDL_Point){
      (start_button->x + (TRIANGLE_FACTOR) * (start_button->radius)),
      (start_button->y)};
  triangle->r = 0;
  triangle->g = 0;
  triangle->b = 0;
  triangle->a = 255;
}

void free_alocatedmemory(Simulator *simulator) {

  free_collision_array(&simulator->collision_array);

  BallNode *current = simulator->balls;
  while (current != NULL) {
    BallNode *next = current->next;
    free(current->ball);
    free(current);
    current = next;
  }
  simulator->balls = NULL;
  simulator->n_balls = 0;

  if (simulator->text_rect != NULL) {
    free(simulator->text_rect);
    simulator->text_rect = NULL;
  }

  if (simulator->titulo != NULL) {
    free(simulator->titulo);
    simulator->titulo = NULL;
  }

  if (simulator->border != NULL) {
    free(simulator->border);
    simulator->border = NULL;
  }

  if (simulator->triangle != NULL) {
    free(simulator->triangle);
    simulator->triangle = NULL;
  }

  if (simulator->start_button != NULL) {
    free(simulator->start_button);
    simulator->start_button = NULL;
  }

  if (simulator->box != NULL) {
    free(simulator->box);
    simulator->box = NULL;
  }

  if (simulator->settings != NULL) {
    free(simulator->settings);
    simulator->settings = NULL;
  }

  if (simulator->mass_center != NULL) {
    free(simulator->mass_center);
    simulator->mass_center = NULL;
  }
}
