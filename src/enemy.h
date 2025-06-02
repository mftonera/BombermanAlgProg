#ifndef ENEMY_H
#define ENEMY_H

#define MAX_ENEMIES 2

typedef struct {
    int x, y;
    int dirX, dirY; // direção atual
    int alive;
    float moveTimer;
} Enemy;

extern Enemy enemies[MAX_ENEMIES];

void InitEnemies(void);
void UpdateEnemies(void);
void DrawEnemies(void);

#endif
