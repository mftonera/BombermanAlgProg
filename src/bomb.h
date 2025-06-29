// src/bomb.h
#ifndef BOMB_H
#define BOMB_H

#include "raylib.h"

#define MAX_BOMBS 10
#define MAX_EXPLOSIONS 50

typedef struct {
    int x, y;
    float timer;
    int active;
} Explosion;

typedef struct {
    int x, y;
    float timer;
    int active;
} Bomb;

void InitBombs(void);
void UpdateBombs(Sound boom);
void DrawBombs(Texture2D bombTexture);
void PlantBomb(int x, int y);

void DrawExplosions(void);
void UpdateExplosions(void);
void AddExplosion(int x, int y);

void DetonateBombAt(int x, int y);

int IsExplosionAt(int x, int y);
int IsBombAt(int x, int y);


extern Bomb bombs[MAX_BOMBS];
extern Explosion explosions[MAX_EXPLOSIONS];


#endif
