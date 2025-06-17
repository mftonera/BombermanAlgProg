// src/level.c
#include "raylib.h"
#include "level.h"
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdio.h>

TileType map[MAP_HEIGHT][MAP_WIDTH];
int exitX, exitY; // <--- DECLARAR AQUI TAMBÉM

void InitLevel(void) {
    srand(time(NULL));

    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (y == 0 || y == MAP_HEIGHT - 1 || x == 0 || x == MAP_WIDTH - 1 || (y % 2 == 0 && x % 2 == 0)) {
                map[y][x] = TILE_PAREDE_INDESTRUTIVEL;
            } else {
                map[y][x] = (rand() % 100 < 30) ? TILE_PAREDE_DESTRUTIVEL : TILE_VAZIO;
            }
        }
    }

    // Garantir áreas seguras iniciais (superior esquerda)
    map[1][1] = TILE_VAZIO;
    map[1][2] = TILE_VAZIO;
    map[2][1] = TILE_VAZIO;

    // --- Nova parte: Salvar a posição da saída, mas manter como parede destrutível inicialmente ---
    int tentativas = 0;
    bool saidaColocada = false;

    // Tenta encontrar uma posição aleatória para a saída que seja uma PAREDE DESTRUTÍVEL
    // E que não esteja na área inicial segura do jogador (para não dar a saída de cara)
    while (!saidaColocada && tentativas < 1000) {
        int tempX = rand() % MAP_WIDTH;
        int tempY = rand() % MAP_HEIGHT;

        if (map[tempY][tempX] == TILE_PAREDE_DESTRUTIVEL &&
            (tempX > 3 || tempY > 3)) // Garante que não esteja nas primeiras 3x3 posições
        {
            exitX = tempX; // Salva a coordenada X da saída
            exitY = tempY; // Salva a coordenada Y da saída
            // O tile map[exitY][exitX] continua sendo TILE_PAREDE_DESTRUTIVEL
            saidaColocada = true;
        }
        tentativas++;
    }

    // Fallback: Se não conseguir encontrar uma parede destrutível aleatoriamente,
    // percorre o mapa para encontrar a primeira parede destrutível e a define como saída.
    if (!saidaColocada) {
        for (int y = MAP_HEIGHT - 2; y >= 0; y--) {
            for (int x = MAP_WIDTH - 2; x >= 0;  x--) {
                if (map[y][x] == TILE_PAREDE_DESTRUTIVEL) {
                    exitX = x;
                    exitY = y;
                    saidaColocada = true;
                    break;
                }
            }
            if (saidaColocada) break;
        }
    }
    // --- Fim da nova parte ---
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

bool CarregarMapaDeArquivo(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return false;

    char linha[MAP_WIDTH + 2]; // +2 por \n e \0
    for (int y = 0; y < MAP_HEIGHT; y++) {
        if (!fgets(linha, sizeof(linha), f)) break;

        for (int x = 0; x < MAP_WIDTH && linha[x] != '\0'; x++) {
            if (linha[x] == 'W') map[y][x] = TILE_PAREDE_INDESTRUTIVEL;
            else if (linha[x] == 'B') map[y][x] = TILE_PAREDE_DESTRUTIVEL;
            else map[y][x] = TILE_VAZIO;
        }
    }

    fclose(f);
    return true;
}


