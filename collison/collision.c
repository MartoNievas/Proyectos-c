#include "raylib.h"
#include <math.h>
#include <stdlib.h>

#define WIDTH 900
#define HEIGHT 600

#define PARTICLES_NUM 30

typedef struct {
  float x;
  float y;
  float r;
  float vx;
  float vy;
  float mass;
} Particle;

Particle *particles[PARTICLES_NUM];

void DrawParticle(Particle *particle) {
  DrawCircle(particle->x, particle->y, particle->r, RED);
}

void UpdateParticle(Particle *particle) {
  particle->x += particle->vx;
  particle->y += particle->vy;

  float x = particle->x;
  float y = particle->y;
  float r = particle->r;

  if (x + r > WIDTH) {
    particle->vx *= -1;
    particle->x = WIDTH - r;
  }
  if (x - r < 0) {
    particle->vx *= -1;
    particle->x = r;
  }
  if (y + r > HEIGHT) {
    particle->vy *= -1;
    particle->y = HEIGHT - r;
  }
  if (y - r < 0) {
    particle->vy *= -1;
    particle->y = r;
  }
}

void InitParticles() {
  for (int i = 0; i < PARTICLES_NUM; i++) {
    particles[i] = malloc(sizeof(Particle));

    Particle *p = particles[i];
    p->r = (float)GetRandomValue(5, 15);
    p->x = (float)GetRandomValue(p->r, WIDTH);
    p->y = (float)GetRandomValue(p->r, HEIGHT);
    p->vx = (float)GetRandomValue(-5, 5);
    p->vy = (float)GetRandomValue(-5, 5);
    p->mass = (float)p->r;
  }
}

void CollisionAllParticles() {
  for (int i = 0; i < PARTICLES_NUM; i++) {
    for (int j = i + 1; j < PARTICLES_NUM; j++) {
      Particle *p1 = particles[i];
      Particle *p2 = particles[j];

      float dx = p2->x - p1->x;
      float dy = p2->y - p1->y;
      float distanceSq = (dx * dx) + (dy * dy);
      float radiusSum = p1->r + p2->r;

      if (distanceSq < (radiusSum * radiusSum)) {
        float dist = sqrt(distanceSq);
        float nx = dx / dist;
        float ny = dy / dist;

        float rvx = p2->vx - p1->vx;
        float rvy = p2->vy - p1->vy;
        float vn = (rvx * nx) + (rvy * ny);

        if (vn > 0)
          continue;

        float restitution = 1.0f;
        float impulse =
            -(1.0f + restitution) * vn / (1.0f / p1->mass + 1.0f / p2->mass);

        p1->vx -= (impulse / p1->mass) * nx;
        p1->vy -= (impulse / p1->mass) * ny;
        p2->vx += (impulse / p2->mass) * nx;
        p2->vy += (impulse / p2->mass) * ny;

        float overlap = radiusSum - dist;
        float totalMass = p1->mass + p2->mass;
        float ratio1 = p2->mass / totalMass;
        float ratio2 = p1->mass / totalMass;

        p1->x -= nx * (overlap * ratio1);
        p1->y -= ny * (overlap * ratio1);
        p2->x += nx * (overlap * ratio2);
        p2->y += ny * (overlap * ratio2);
      }
    }
  }
}

void ClearParticles() {

  for (int i = 0; i < PARTICLES_NUM; i++) {
    free(particles[i]);
  }
}

void DrawParticles() {
  for (int i = 0; i < PARTICLES_NUM; i++) {
    DrawParticle(particles[i]);
  }
}

void UpdateParticles() {
  for (int i = 0; i < PARTICLES_NUM; i++) {
    UpdateParticle(particles[i]);
  }
}

int main(void) {

  InitWindow(WIDTH, HEIGHT, "Particle Collision Simulation");
  InitParticles();
  SetTargetFPS(60);
  while (!WindowShouldClose()) {

    BeginDrawing();
    DrawFPS(10, 10);
    ClearBackground(BLACK);
    UpdateParticles();
    CollisionAllParticles();
    DrawParticles();
    EndDrawing();
  }
  CloseWindow();
  ClearParticles();
  return 1;
}
