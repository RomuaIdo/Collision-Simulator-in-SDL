#include "../include/main.h"
#include "../include/input.h"
#include "../include/objects.h"

int main(int argc, char *argv[]) {
  Simulator *simulator;
  simulator = (Simulator *)malloc(sizeof(Simulator));
  simulator->menu.titulo = (Text *)malloc(sizeof(Text));
  strcpy(simulator->menu.titulo->text_buffer, "Collision Simulator");
  if (!simulator) {
    fprintf(stderr, "Memory allocation failed\n");
    return EXIT_FAILURE;
  }

  printf("Digite o coeficiente de restituição: ");
  scanf("%f", &simulator->world.CR);

  simulator->running = initialize(simulator);
  setup(simulator);
  setup_initial_screen(simulator);
  while (simulator->running) {
    if (simulator->state == INITIAL_SCREEN) {
      process_initial_screen_input(&simulator->menu, &simulator->world,
                                   &simulator->state, &simulator->running);
      update(simulator);
      render(simulator);
    } else if (simulator->state == RUNNING || simulator->state == PAUSED) {
      process_input(simulator->settings, &simulator->world, &simulator->state,
                    &simulator->running);
      update(simulator);
      render(simulator);
    }
  }

  destroy_window(&simulator->window_config, &simulator->resources,
                 simulator->menu.titulo);

  free_alocatedmemory(simulator);
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

  simulator->window_config.window =
      SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_BORDERLESS);
  if (!simulator->window_config.window) {
    fprintf(stderr, "Error creating SDL window: %s\n", SDL_GetError());
    return FALSE;
  }

  simulator->window_config.renderer =
      SDL_CreateRenderer(simulator->window_config.window, -1, 0);

  if (!simulator->window_config.renderer) {
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
  simulator->world.mass_center = (MassCenter *)malloc(sizeof(MassCenter));
  *(simulator->world.mass_center) = (MassCenter){0, 0, 0, 0, 0, 5};
  simulator->world.box = (SDL_Rect *)malloc(sizeof(SDL_Rect));
  *(simulator->world.box) =
      (SDL_Rect){50, 50, (BOX_FACTOR_X * SCREEN_WIDTH) - 50,
                 (BOX_FACTOR_Y * SCREEN_HEIGHT) - 50};
  simulator->world.border = (Border *)malloc(sizeof(Border));
  *(simulator->world.border) = (Border){0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};

  init_ball_array(&simulator->world.ball_array, 16);
  init_collision_array(&simulator->world.collision_array, 128);
  simulator->resources.collision_sound = loadSound("./assets/collision.wav");

  for (int i = 0; i < 4; i++) {
    add_random_ball(&simulator->world.ball_array, simulator->world.border);
  }

  simulator->last_frame_time = 0;
  simulator->state = INITIAL_SCREEN;
}

void setup_initial_screen(Simulator *simulator) {
  simulator->menu.titulo->text_rect = (SDL_Rect *)malloc(sizeof(SDL_Rect));
  simulator->menu.titulo->color = (SDL_Color){255, 255, 255, 255};
  simulator->menu.titulo->size = 48;
  load_font(simulator->menu.titulo, &simulator->resources,
            "./assets/stardew-valley.ttf");
  create_text_texture(simulator->menu.titulo, &simulator->resources,
                      &simulator->window_config);
  simulator->menu.triangle = (Triangle *)malloc(sizeof(Triangle));
  simulator->menu.start_button = (Circle_Button *)malloc(sizeof(Circle_Button));
  simulator->menu.start_button->x = (float)(SCREEN_WIDTH / 2.f);
  simulator->menu.start_button->y = (float)(SCREEN_HEIGHT / 2.f);
  simulator->menu.start_button->radius = 50;
  simulator->menu.start_button->r = 255;
  simulator->menu.start_button->g = 255;
  simulator->menu.start_button->b = 255;
  simulator->menu.start_button->a = 255;

  Circle_Button *start_button = simulator->menu.start_button;
  Triangle *triangle = simulator->menu.triangle;

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

  free_collision_array(&simulator->world.collision_array);
  free_ball_array(&simulator->world.ball_array);
  


  if (simulator->menu.titulo && simulator->menu.titulo->text_rect != NULL) {
    free(simulator->menu.titulo->text_rect);
    simulator->menu.titulo->text_rect = NULL;
  }

  if (simulator->menu.titulo != NULL) {
    free(simulator->menu.titulo);
    simulator->menu.titulo = NULL;
  }

  if (simulator->world.border != NULL) {
    free(simulator->world.border);
    simulator->world.border = NULL;
  }

  if (simulator->menu.triangle != NULL) {
    free(simulator->menu.triangle);
    simulator->menu.triangle = NULL;
  }

  if (simulator->menu.start_button != NULL) {
    free(simulator->menu.start_button);
    simulator->menu.start_button = NULL;
  }

  if (simulator->world.box != NULL) {
    free(simulator->world.box);
    simulator->world.box = NULL;
  }

  if (simulator->settings != NULL) {
    free(simulator->settings);
    simulator->settings = NULL;
  }

  if (simulator->world.mass_center != NULL) {
    free(simulator->world.mass_center);
    simulator->world.mass_center = NULL;
  }
}
