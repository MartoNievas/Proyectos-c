#include "lib/tinyexpr.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#define WIDTH 900
#define HEIGHT 900
#define W_MID WIDTH / 2
#define H_MID HEIGHT / 2
#define X_START -20
#define X_END 20
#define Y_START -20
#define Y_END 20

/*Funcion aux para dibujar texto*/
void draw_text(SDL_Surface *psurface, TTF_Font *font, int x, int y,
               const char *text) {
  SDL_Color color = {255, 255, 255, 255};
  SDL_Surface *text_surface = TTF_RenderText_Blended(font, text, color);

  if (!text_surface)
    return;
  SDL_Rect dst = {x, y, text_surface->w, text_surface->h};
  SDL_BlitSurface(text_surface, NULL, psurface, &dst);

  SDL_FreeSurface(text_surface);
}

/*Funcion para dibujar valores en los ejes*/
void draw_grid_values(SDL_Surface *psurface, SDL_Window *pwindow) {
  TTF_Font *font =
      TTF_OpenFont("/usr/share/fonts/TTF/JetBrainsMonoNerdFont-Regular.ttf", 9);

  if (!font) {
    printf("TTF_OpenFont failed: %s\n", TTF_GetError());
    return;
  }
  char buffer[16];

  double x_scale = WIDTH / (double)(X_END - X_START);
  double y_scale = HEIGHT / (double)(Y_END - Y_START);

  Uint32 color = SDL_MapRGB(psurface->format, 255, 255, 255);

  /*EJE X*/
  SDL_Rect line_x = (SDL_Rect){0, 0, 2, 4};
  SDL_Rect line_y = (SDL_Rect){0, 0, 4, 2};
  for (int x = X_START + 1; x <= X_END - 1; x++) {
    snprintf(buffer, sizeof(buffer), "%d", x);
    int sx = W_MID + (int)(x * x_scale);
    if (x == 0)
      continue;
    line_x.x = sx + 2;
    line_x.y = H_MID - 2;
    SDL_FillRect(psurface, &line_x, color);

    if (x > 0) {
      draw_text(psurface, font, sx, H_MID + 6, buffer);
      continue;
    }
    draw_text(psurface, font, sx - 6, H_MID + 6, buffer);
  }

  /*EJE y*/
  for (int y = Y_START + 1; y <= Y_END - 1; y++) {
    if (y == 0)
      continue;
    int sy = H_MID - (int)(y * y_scale);

    line_y.y = sy - 2;
    line_y.x = W_MID + 2;
    SDL_FillRect(psurface, &line_y, color);

    snprintf(buffer, sizeof(buffer), "%d", y);
    draw_text(psurface, font, W_MID + 6, sy - 6, buffer);
  }
  TTF_CloseFont(font);
  SDL_UpdateWindowSurface(pwindow);
}

/*Funcion para dibujar el eje*/
void draw_grid_at_cordinate(SDL_Surface *psurface, SDL_Rect *prect,
                            Uint32 color) {
  int x_grid = psurface->w / 2 +
               prect->x; // Lo tenemos que dibujar en la mitad de la pantalla
  int y_grid = psurface->h / 2 + prect->y;
  SDL_Rect shifted_rect = (SDL_Rect){x_grid, y_grid, prect->w, prect->h};
  SDL_FillRect(psurface, &shifted_rect, color);
}
void draw_grid(SDL_Surface *psurface, SDL_Window *pwindow) {
  Uint32 color = 0x9c9c9c;
  // eje x
  SDL_Rect x_pixel;
  x_pixel.w = WIDTH;
  x_pixel.h = 2;
  x_pixel.x = -W_MID;
  x_pixel.y = 0;

  draw_grid_at_cordinate(psurface, &x_pixel, color);

  SDL_Rect y_pixel;
  y_pixel.w = 2;
  y_pixel.h = HEIGHT;
  y_pixel.y = -H_MID;
  y_pixel.x = 0;

  draw_grid_at_cordinate(psurface, &y_pixel, color);

  SDL_UpdateWindowSurface(pwindow);
}
void draw_function(SDL_Surface *psurface, SDL_Window *pwindow, te_expr *pexpr,
                   double *x_var) {
  double y, x;
  double step = 0.00001;
  double x_scale = (double)WIDTH / (double)(X_END - X_START);
  double y_scale = (double)HEIGHT / (double)(Y_END - Y_START);
  Uint32 color = SDL_MapRGB(psurface->format, 0, 255, 255);
  SDL_Rect pixel = {0, 0, 2, 2};

  for (x = X_START; x <= X_END; x += step) {
    *x_var = x;
    y = te_eval(pexpr);
    int sx = W_MID + (int)(x * x_scale);
    int sy = H_MID - (int)(y * y_scale);

    if (sx >= 0 && sx < WIDTH && sy >= 0 && sy < HEIGHT) {
      pixel.x = sx;
      pixel.y = sy;
      SDL_FillRect(psurface, &pixel, color);
    }
  }

  SDL_UpdateWindowSurface(pwindow);
}
int main(int argc, char *argv[]) {
  printf("Function Plotter, aqui vamos!!\n");

  if (argc < 2) {
    printf("Uso: %s \"expresion\"\n", argv[0]);
    return 1;
  }
  // iniciamos SDL
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    printf("SDL init error: %s\n", SDL_GetError());
    return -1;
  }
  // iniciamos SDL_TTF
  if (TTF_Init() == -1) {
    printf("Error: TTF init failed%s\n", TTF_GetError());
    return -1;
  }

  SDL_Window *pwindow =
      SDL_CreateWindow("Function Plotter", SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

  if (!pwindow) {
    printf("SDL window error: %s\n", SDL_GetError());
    return -1;
  }
  SDL_Surface *psurface = SDL_GetWindowSurface(pwindow);

  if (!psurface) {
    printf("Surface error: %s\n", SDL_GetError());
    SDL_Quit();
    return 0;
  }
  Uint32 bg = SDL_MapRGB(psurface->format, 32, 32, 32);
  SDL_FillRect(psurface, NULL, bg);

  // Dibujamos el eje cartesianso
  draw_grid(psurface, pwindow);
  // Y su valores
  draw_grid_values(psurface, pwindow);
  double x;

  te_variable vars[] = {{"x", &x, 0, 0}};
  char *expression = argv[1];
  int err;
  te_expr *expr = te_compile(expression, vars, 1, &err);

  if (!expr) {
    printf("Error en la expresión en la posición %d\n", err);
    return 1;
  }
  draw_function(psurface, pwindow, expr, &x);
  SDL_UpdateWindowSurface(pwindow);
  int running = 1;
  SDL_Event event;
  while (running) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        running = 0;
        break;
      }
    }
    SDL_Delay(16);
  }
  SDL_DestroyWindow(pwindow);
  SDL_Quit();
  return 0;
}
