
#include "raylib.h"

#define WIDTH 900
#define HEIGHT 600

#define MASS_SIZE 100
#define FLOOR_HEIGHT 250

#define SPRING_SEGMENTS 24
#define SPRING_WIDTH 25

#define START_V 200.0f
#define START_K 40.0f
#define MASS 1.0f
#define FRICTION 6.0f

float x = 5.0f;
float v = START_V;
float a = 10.0f;

float k = START_K;
float m = 10;
float damping = 1;

float x0 = 300.0f;

Vector2 anchor = {0, HEIGHT - FLOOR_HEIGHT - MASS_SIZE / 2};

void update_physics() {
  float dt = GetFrameTime();

  float Fspring = -k * x;
  float Fdamp = -damping * v;

  a = (Fspring + Fdamp) / m;

  v += a * dt;
  x += v * dt;
}

void draw_floor() {
  DrawRectangle(0, HEIGHT - FLOOR_HEIGHT, WIDTH, FLOOR_HEIGHT, GRAY);
}

void draw_mass() {
  DrawRectangle(anchor.x + x0 + x - MASS_SIZE / 2, anchor.y - MASS_SIZE / 2,
                MASS_SIZE, MASS_SIZE, RED);
}

void draw_spring() {
  float mass_left = anchor.x + x0 + x - MASS_SIZE / 2;

  Vector2 start = {anchor.x + 20, anchor.y};
  Vector2 end = {mass_left - 20, anchor.y};

  DrawLine(anchor.x, anchor.y, start.x, start.y, WHITE);

  Vector2 prev = start;
  float dx = (end.x - start.x) / SPRING_SEGMENTS;

  for (int i = 1; i <= SPRING_SEGMENTS; i++) {
    Vector2 curr;
    curr.x = start.x + dx * i;
    curr.y = start.y + ((i % 2) ? SPRING_WIDTH : -SPRING_WIDTH);
    DrawLineV(prev, curr, WHITE);
    prev = curr;
  }

  DrawLine(end.x, end.y, mass_left, anchor.y, WHITE);
}

int main(void) {
  InitWindow(WIDTH, HEIGHT, "Spring Horizontal");
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    update_physics();

    BeginDrawing();
    ClearBackground(BLACK);

    DrawFPS(10, 10);
    draw_floor();
    draw_spring();
    draw_mass();

    EndDrawing();
  }

  CloseWindow();
  return 0;
}
