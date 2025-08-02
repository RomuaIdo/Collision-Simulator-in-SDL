#include "../include/main.h"


State state = INITIAL_SCREEN;
int running = FALSE;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
ShowRender *show_render;
MassCenter *mass_center;
float CR = 1.0;
Mix_Chunk *collision_sound;
int last_frame_time;
Border *border;
SDL_Rect *box;
Texto *titulo;
Circle_Button *start_button;
SDL_Rect *text_rect;
Triangle *triangle;
BallNode *balls = NULL;
int n_balls = 0;
char text[20] = "Collision Simulator";

int main(int argc, char *argv[]) {
  printf("Digite o coeficiente de restituição: ");
  scanf("%f", &CR);
  running = initialize();
  setup();
  setup_initial_screen();
  while (running) {
    if (state == INITIAL_SCREEN) {
      process_initial_screen_input();
      update();
      render();
    } else if (state == RUNNING || state == PAUSED) {
      process_input();
      update();
      render();
    }
  }
  destroy_window();

  return 0;
}

int initialize(void) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
    return FALSE;
  }

  if (TTF_Init() != 0) {
    fprintf(stderr, "Error initializing SDL_ttf: %s\n", TTF_GetError());
    return FALSE;
  }

  window =
      SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                       SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_BORDERLESS);
  if (!window) {
    fprintf(stderr, "Error creating SDL window: %s\n", SDL_GetError());
    return FALSE;
  }

  renderer = SDL_CreateRenderer(window, -1, 0);

  if (!renderer) {
    fprintf(stderr, "Error creating SDL renderer: %s\n", SDL_GetError());
    return FALSE;
  }

  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
    fprintf(stderr, "Error initializing SDL_mixer: %s\n", Mix_GetError());
    return FALSE;
  }

  return TRUE;
}

void setup() {
  srand(time(NULL));
  int i;
  // Mix_AllocateChannels(32);
  show_render = (ShowRender *)malloc(sizeof(ShowRender));
  *show_render = (ShowRender){FALSE, FALSE, FALSE, FALSE};
  mass_center = (MassCenter *)malloc(sizeof(MassCenter));
  *mass_center = (MassCenter){0, 0, 0, 0, 0, 5};
  box = (SDL_Rect *)malloc(sizeof(SDL_Rect));
  *box = (SDL_Rect){50, 50, (BOX_FACTOR_X * SCREEN_WIDTH) - 50,
                    (BOX_FACTOR_Y * SCREEN_HEIGHT) - 50};
  collision_sound = loadSound("./assets/collision.wav");

  for (i = 0; i < 4; i++) {
    Ball ball = generate_random_ball();
    add_ball(ball);
    mass_center->total_mass += ball.mass;
  }

  last_frame_time = 0;
  state = INITIAL_SCREEN;
}



void setup_initial_screen(void) {
  text_rect = (SDL_Rect *)malloc(sizeof(SDL_Rect));
  titulo = (Texto *)malloc(sizeof(Texto));
  titulo->color = (SDL_Color){255, 255, 255, 255};
  titulo->size = 48;
  load_font("./assets/stardew-valley.ttf", titulo->size);
  create_text_texture(renderer, text, titulo->color);
  border = (Border *)malloc(sizeof(Border));
  *border = (Border){0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
  triangle = (Triangle *)malloc(sizeof(Triangle));
  start_button = (Circle_Button *)malloc(sizeof(Circle_Button));
  start_button->x = (float)(SCREEN_WIDTH / 2.f);
  start_button->y = (float)(SCREEN_HEIGHT / 2.f);
  start_button->radius = 50;
  start_button->r = 255;
  start_button->g = 255;
  start_button->b = 255;
  start_button->a = 255;
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

void free_alocatedmemory(void) {
  BallNode *current = balls;
  while (current != NULL) {
    BallNode *next = current->next;
    free(current);
    current = next;
  }
  balls = NULL;
  n_balls = 0;

  free(text_rect);
  free(titulo);
  free(border);
  free(triangle);
  free(start_button);
  if (box != NULL) {
    free(box);
  }
  if (show_render != NULL) {
    free(show_render);
  }
  if (mass_center != NULL) {
    free(mass_center);
  }
}
