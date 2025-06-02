// src/level.c
#include "raylib.h"
#include "level.h"
#include <stdlib.h>
#include <time.h>

TileType map[MAP_HEIGHT][MAP_WIDTH];

void InitLevel(void) {
    srand(time(NULL));

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (y == 0 || y == MAP_HEIGHT - 1 || x == 0 || x == MAP_WIDTH - 1 || (y % 2 == 0 && x % 2 == 0)) {
                map[y][x] = TILE_PAREDE_INDESTRUTIVEL;
            } else {
                // 30% de chance de parede destrutível, senão vazio
                map[y][x] = (rand() % 100 < 30) ? TILE_PAREDE_DESTRUTIVEL : TILE_VAZIO;
            }
        }
    }

    // Garantir áreas seguras iniciais (superior esquerda)
    map[1][1] = TILE_VAZIO;
    map[1][2] = TILE_VAZIO;
    map[2][1] = TILE_VAZIO;
}

void DrawLevel(void) {
    int offsetX = (GetScreenWidth() - (MAP_WIDTH * TILE_SIZE)) / 2;
    int offsetY = (GetScreenHeight() - (MAP_HEIGHT * TILE_SIZE)) / 2;

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            Rectangle tile = {
                offsetX + x * TILE_SIZE,
                offsetY + y * TILE_SIZE,
                TILE_SIZE, TILE_SIZE
            };

            Color color;
            switch (map[y][x]) {
                case TILE_VAZIO:              color = LIGHTGRAY; break;
                case TILE_PAREDE_INDESTRUTIVEL: color = DARKGRAY;  break;
                case TILE_PAREDE_DESTRUTIVEL:   color = BROWN;     break;
                case TILE_SAIDA:               color = BLUE;      break;
                case TILE_POWERUP_BOMBA:       color = GREEN;     break;
                case TILE_POWERUP_EXPLOSAO:    color = RED;       break;
            }

            DrawRectangleRec(tile, color);
            DrawRectangleLinesEx(tile, 1, BLACK);
        }
    }
}
