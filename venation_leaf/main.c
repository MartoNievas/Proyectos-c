#include <raylib.h>
#include <raymath.h>
#include <stddef.h>
#define NOB_STRIP_PREFIX
#include "nob.h"
#define WIDTH 800
#define HEIGHT 600

typedef struct {
  size_t *items;
  size_t count;
  size_t capacity;
} Index;

typedef struct {
  Vector2 *items;
  size_t count;
  size_t capacity;
} Attractors;

typedef struct {
  Vector2 position;
  Vector2 direction;
} Vein;

typedef struct {
  Vein *items;
  size_t count;
  size_t capacity;
} Veins;

Attractors auxins = {0};
Veins veins = {0};

#define VEIN_RADIUS 5
#define VEIN_COLOR GREEN
#define VEIN_CORE_COLOR WHITE
#define AUXINS_RATE 20
#define AUXIN_RADIUS VEIN_RADIUS
#define AUXIN_COLOR ORANGE
#define AUXIMITY 20

void spray_auxins() {
  int w = WIDTH;
  int h = HEIGHT;

  for (size_t i = 0; i < AUXINS_RATE; i++) {
    Vector2 p;

    p.x = GetRandomValue(0, w);
    p.y = GetRandomValue(0, h);
    da_append(&auxins, p); // Para agregar al final del arrelgo
  }
}

void kill_auxins_by_auximity(void) {
  static Index to_remove = {0};
  to_remove.count = 0;
  da_foreach(Vector2, auxin, &auxins) {
    size_t index = auxin - auxins.items;
    da_foreach(Vein, vein, &veins) {
      if (Vector2Distance(*auxin, vein->position) <= AUXIMITY) {
        da_append(&to_remove, index);
        break;
      }
    }
  }
  while (to_remove.count > 0) {
    da_remove_unordered(&auxins, da_last(&to_remove));
    to_remove.count -= 1;
  }
}

void calculate_growth_directions(void) {
  if (veins.count > 0) {
    da_foreach(Vein, vein, &veins) { vein->direction = Vector2Zero(); }

    da_foreach(Vector2, auxin, &auxins) {
      Vein *cvein = &veins.items[0];
      for (size_t index = 1; index < veins.count; ++index) {
        Vein *vein = &veins.items[index];
        Vector2 a = vein->position;
        Vector2 b = cvein->position;
        if (Vector2Distance(a, *auxin) < Vector2Distance(b, *auxin)) {
          cvein = vein;
        }
      }

      cvein->direction = Vector2Add(cvein->direction,
                                    Vector2Subtract(*auxin, cvein->position));
    }

    da_foreach(Vein, vein, &veins) {
      vein->direction = Vector2Normalize(vein->direction);
    }
  }
}

void grow_new_veins(void) {
  static Attractors new_veins = {0};
  new_veins.count = 0;
  da_foreach(Vein, vein, &veins) {
    if (vein->direction.x == 0.0f && vein->direction.y == 0.0f)
      continue;
    Vector2 new_vein = {0};
    new_vein.x = vein->position.x + vein->direction.x * VEIN_RADIUS * 2;
    new_vein.y = vein->position.y + vein->direction.y * VEIN_RADIUS * 2;
    da_append(&new_veins, new_vein);
  }
  da_foreach(Vector2, position, &new_veins) {
    Vein new_vein = {
        .position = *position,
    };
    da_append(&veins, new_vein);
  }
}

int main(void) {

  InitWindow(WIDTH, HEIGHT, "Leaf Venation");
  SetTargetFPS(60);

  {
    int w = WIDTH;
    int h = HEIGHT;
    Vein vein = {0};
    vein.position.x = w / 2.0f;
    vein.position.y = h * 2.0f / 3.0f;
    da_append(&veins, vein);
  }

  spray_auxins();
  kill_auxins_by_auximity();

  while (!WindowShouldClose()) {

    if (IsKeyPressed(KEY_SPACE)) {
      calculate_growth_directions();
      grow_new_veins();
      kill_auxins_by_auximity();
      spray_auxins();
      kill_auxins_by_auximity();
    }

    BeginDrawing();
    {
      ClearBackground(BLACK);

      da_foreach(Vein, vein, &veins) {
        DrawCircle(vein->position.x, vein->position.y, VEIN_RADIUS, VEIN_COLOR);
        DrawCircle(vein->position.x, vein->position.y, VEIN_RADIUS / 2.0f,
                   VEIN_CORE_COLOR);

        DrawLineV(vein->position,
                  Vector2Add(vein->position, Vector2Scale(vein->direction, 20)),
                  PURPLE);
      }

      for (size_t i = 0; i < auxins.count; i++) {
        Vector2 p = auxins.items[i];
        DrawCircle(p.x, p.y, AUXIN_RADIUS, AUXIN_COLOR);

        if (0) {
          DrawRing(p, AUXIMITY, AUXIMITY + 2, 0, 360, 69, AUXIN_COLOR);
        }
      }
    }
    EndDrawing();
  }

  CloseWindow();
  return 1;
}
