// src/level.h
#ifndef LEVEL_H
#define LEVEL_H

#include <stdbool.h>

#define MAP_WIDTH 15
#define MAP_HEIGHT 15
#define TILE_SIZE 32

bool CarregarMapaDeArquivo(const char *filename);

typedef enum {
    TILE_VAZIO,
    TILE_PAREDE_INDESTRUTIVEL,
    TILE_PAREDE_DESTRUTIVEL,
    TILE_SAIDA,
    TILE_POWERUP_BOMBA,
    TILE_POWERUP_EXPLOSAO
} TileType;

extern TileType map[MAP_HEIGHT][MAP_WIDTH];
extern int exitX, exitY;

void InitLevel(void);
void DrawLevel(void);

#endif
