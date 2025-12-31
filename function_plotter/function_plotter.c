#include <stdio.h>
#include <string.h>
#include "lib/tinyexpr.h"
#include <SDL2/SDL.h>
#define WIDTH 900
#define HEIGHT 600
#define W_MID WIDTH/2
#define H_MID HEIGHT/2
#define X_START -10
#define X_END 10
#define Y_START -5
#define Y_END 5
/*Funcion para dibujar el eje*/
void draw_grid_at_cordinate(SDL_Surface* psurface, SDL_Rect* prect, Uint32 color) {
    int x_grid = psurface->w/2 + prect->x; //Lo tenemos que dibujar en la mitad de la pantalla 
    int y_grid = psurface->h/2 + prect->y;
    SDL_Rect shifted_rect = (SDL_Rect) {x_grid,y_grid,prect->w,prect->h};
    SDL_FillRect(psurface,&shifted_rect,color);    
}
void draw_grid(SDL_Surface* psurface, SDL_Window *pwindow) {
  Uint32 color = 0x9c9c9c; 
  //eje x
  SDL_Rect x_pixel;
  x_pixel.w = WIDTH;
  x_pixel.h = 2;
  x_pixel.x = -W_MID;
  x_pixel.y = 0;
 
  draw_grid_at_cordinate(psurface,&x_pixel,color);
  
  SDL_Rect y_pixel;
  y_pixel.w = 2;
  y_pixel.h = HEIGHT;
  y_pixel.y = -H_MID;
  y_pixel.x = 0;
  
  draw_grid_at_cordinate(psurface,&y_pixel,color);
  
  SDL_UpdateWindowSurface(pwindow);
}
void draw_function(SDL_Surface* psurface, SDL_Window* pwindow,
                   te_expr* pexpr,double* x_var)
{
    double y,x;
    double step = 0.00001;
    double x_scale = WIDTH  / (X_END - X_START);
    double y_scale = HEIGHT / (Y_END - Y_START);
    Uint32 color = SDL_MapRGB(psurface->format, 255, 0, 0);
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
int main(int argc, char *argv[])
{
  printf("Function Plotter, aqui vamos!!\n");
  
  if (argc < 2) {
  printf("Uso: %s \"expresion\"\n", argv[0]);
    return 1;
  }
  
  if (SDL_Init(SDL_INIT_VIDEO) != 0 ) {
    printf("SDL init error: %s\n", SDL_GetError());
    return -1;
  }
  
  SDL_Window* pwindow = SDL_CreateWindow("Function Plotter",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED
                                         ,WIDTH,HEIGHT,SDL_WINDOW_SHOWN);
  
  if (!pwindow) {
    printf("SDL window error: %s\n",SDL_GetError());
    return -1;  
  
  } 
  SDL_Surface* psurface = SDL_GetWindowSurface(pwindow);
  
  if (!psurface) {
    printf("Surface error: %s\n", SDL_GetError());
    SDL_Quit();
    return 0;
  } 
  //Dibujamos el eje cartesianso
  draw_grid(psurface,pwindow); 
  
  double x;

  te_variable vars[] = {{"x",&x,0,0}};
  char* expression = argv[1]; 
  int err;
  te_expr* expr = te_compile(expression,vars,1,&err);
  
  if (!expr) {
    printf("Error en la expresión en la posición %d\n", err);
    return 1;
  }
  draw_function(psurface,pwindow,expr,&x);
  SDL_UpdateWindowSurface(pwindow);
  int running = 1;
  SDL_Event event; 
  while(running) {
    while(SDL_PollEvent(&event)) {
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
