#ifndef LEVEL_H
#define LEVEL_H

#include "raylib.h"

#define MAP_WIDTH 15
#define MAP_HEIGHT 15
#define TILE_SIZE 32 // Tamanho de cada tile em pixels

// Enumeração dos tipos de tiles
typedef enum
{
    TILE_VAZIO,
    TILE_PAREDE_INDESTRUTIVEL,
    TILE_PAREDE_DESTRUTIVEL,
    TILE_POWERUP_BOMBA,
    TILE_POWERUP_EXPLOSAO,
    TILE_SAIDA
} TileType;

// Declaração da matriz do mapa
// Map => matriz bidimensional de TileType
extern TileType map[MAP_HEIGHT][MAP_WIDTH];
extern int exitX, exitY;

void InitLevel(void);
void DrawLevel(Texture2D floorTexture,
                 Texture2D destructibleTexture,
                 Texture2D wallTexture,
                 Texture2D exitTexture,
                 Texture2D powerupBombTexture,
                 Texture2D powerupRangeTexture);

bool CarregarMapaDeArquivo(const char *filename);

#endif