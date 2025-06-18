#ifndef ENEMY_H
#define ENEMY_H

#include "raylib.h"

#define MAX_ENEMIES 2

typedef struct {
    int x, y;
    int dirX, dirY; // direção atual
    int alive;
    float moveTimer;
    int facingDirection;
} Enemy;

extern Enemy enemies[MAX_ENEMIES];

void InitEnemies(void);
void UpdateEnemies(void);
void DrawEnemies(Texture2D enemyTexture);
void UnloadEnemies(void);

#endif