#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_video.h>
#include <stdbool.h>
#include <stdio.h>

/*Defines use for SLD window */
#define WIDTH 900
#define HIGHT 600
#define BG_COLOR 0x00000000
#define TARGET_FPS 120

/*Defines use to for aux funct*/
#define COLOR_RECT_SIZE 40
#define COLOR_START 0x00ff0000
#define MAX_RADIUS 40

/*Stack for undo*/
#define MAX_UNDO 100
SDL_Surface *undo_stack[MAX_UNDO];
int stack_pointer = -1;

/*Global variables to use for set main color and size of paint line*/
Uint32 color_pallet[] = {0x00000000, 0x00ff0000, 0x0000ff00, 0x000000ff,
                         0x00ffff00, 0x00ff00ff, 0x00ffffff, 0x0000ffff};
int color_pallet_size = sizeof(color_pallet) / sizeof(color_pallet[0]);

Uint32 color = COLOR_START;

int CIRCLE_RADIUS = 20;

/*Save UNDO for perform_undo*/
void push_undo(SDL_Surface *current) {
  if (stack_pointer == MAX_UNDO - 1) {
    SDL_FreeSurface(undo_stack[0]);
    for (int i = 0; i < MAX_UNDO - 1; i++) {
      undo_stack[i] = undo_stack[i + 1];
    }
    stack_pointer--;
  }

  stack_pointer++;
  undo_stack[stack_pointer] = SDL_ConvertSurface(current, current->format, 0);
}

/*Handler CTRL + Z*/
void perform_undo(SDL_Surface *psurface, SDL_Window *pwindow) {

  if (stack_pointer <= 0) {
    return;
  }

  SDL_BlitSurface(undo_stack[stack_pointer], NULL, psurface, NULL);
  SDL_UpdateWindowSurface(pwindow);

  SDL_FreeSurface(undo_stack[stack_pointer]);
  stack_pointer--;
}

/*Aux function for draw pallete changer*/
void draw_color_pallet(SDL_Surface *psurface) {

  struct SDL_Rect color_rect =
      (SDL_Rect){0, 0, COLOR_RECT_SIZE, COLOR_RECT_SIZE};

  for (int i = 0; i < color_pallet_size; i++) {
    Uint32 color = color_pallet[i];
    color_rect.x = COLOR_RECT_SIZE * i;
    SDL_FillRect(psurface, &color_rect, color);
  }
}

void draw_circle(SDL_Surface *psurface, int center_x, int center_y, int radius,
                 Uint32 color) {

  struct SDL_Rect pixel = (SDL_Rect){0, 0, 1, 1};
  for (int x = center_x - radius; x < center_x + radius; x++) {

    for (int y = center_y - radius; y < center_y + radius; y++) {

      int distance_from_center =
          ((x - center_x) * (x - center_x) + (y - center_y) * (y - center_y));
      if (distance_from_center <= radius * radius) {
        pixel.x = x;
        pixel.y = y;
        SDL_FillRect(psurface, &pixel, color);
      }
    }
  }
}

/*Auxiliar function for change main color*/
void change_main_color(int x, int y) {

  if (x <= color_pallet_size * COLOR_RECT_SIZE && y <= COLOR_RECT_SIZE) {
    int color_index = x / COLOR_RECT_SIZE;
    color = color_pallet[color_index];
  }
}

/*Handle events*/
void catch_events(SDL_Window *pwindow, SDL_Surface *psurface, SDL_Event *event,
                  int *x, int *y, bool *draw, bool *running) {
  const Uint8 *state = SDL_GetKeyboardState(NULL);

  switch (event->type) {
  case SDL_QUIT:
    *running = false;
    break;

  case SDL_MOUSEMOTION:
    *x = event->motion.x;
    *y = event->motion.y;
    break;

  case SDL_MOUSEBUTTONDOWN:
    if (event->button.button == SDL_BUTTON_LEFT) {

      push_undo(psurface);
      *draw = true;
      *x = event->button.x;
      *y = event->button.y;
      change_main_color(*x, *y);
    }
    break;

  case SDL_MOUSEBUTTONUP:
    if (event->button.button == SDL_BUTTON_LEFT) {

      *draw = false;
    }
    break;

  case SDL_KEYDOWN:
    if (state[SDL_SCANCODE_LCTRL]) {
      if (event->key.keysym.scancode == SDL_SCANCODE_KP_PLUS ||
          event->key.keysym.scancode == SDL_SCANCODE_EQUALS) {

        if (CIRCLE_RADIUS < MAX_RADIUS)
          CIRCLE_RADIUS++;
      } else if (event->key.keysym.scancode == SDL_SCANCODE_KP_MINUS ||
                 event->key.keysym.scancode == SDL_SCANCODE_MINUS) {

        if (CIRCLE_RADIUS > 0)
          CIRCLE_RADIUS--;
      } else if (event->key.keysym.sym == SDLK_z) {
        perform_undo(psurface, pwindow);
      }
    }
    break;
  }
}

int main(void) {

  bool running = true;
  bool draw = false;
  SDL_Event event;
  int x, y;
  printf("Welcome to the Paint Clone\n");

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("Error: %s\n", SDL_GetError());
    return 0;
  }

  SDL_Window *pwindow =
      SDL_CreateWindow("Ultimate Paint", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, WIDTH, HIGHT, SDL_WINDOW_SHOWN);

  if (!pwindow) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create surface: %s\n",
                 SDL_GetError());
    return 0;
  }

  SDL_Surface *psurface = SDL_GetWindowSurface(pwindow);

  if (!psurface) {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create surface: %s\n",
                 SDL_GetError());
    return 0;
  }

  SDL_FillRect(psurface, NULL, BG_COLOR);
  draw_color_pallet(psurface);
  SDL_UpdateWindowSurface(pwindow);

  undo_stack[stack_pointer] = SDL_ConvertSurface(psurface, psurface->format, 0);
  stack_pointer++;
  while (running) {

    while (SDL_PollEvent(&event)) {
      catch_events(pwindow, psurface, &event, &x, &y, &draw, &running);

      if (draw) {
        draw_circle(psurface, x, y, CIRCLE_RADIUS, color);
        draw_color_pallet(psurface);
        SDL_UpdateWindowSurface(pwindow);
      }
    }
  }
  /*Clean stack at the end */
  for (int i = 0; i <= stack_pointer; i++) {
    if (undo_stack[i])
      SDL_FreeSurface(undo_stack[i]);
  }
  SDL_DestroyWindow(pwindow);
  SDL_Quit();

  return 1;
}
