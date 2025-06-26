#include "level.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

TileType map[MAP_HEIGHT][MAP_WIDTH];
int exitX, exitY;

void InitLevel(void)
{
    srand(time(NULL));

    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            if (y == 0 || y == MAP_HEIGHT - 1 || x == 0 || x == MAP_WIDTH - 1 || (y % 2 == 0 && x % 2 == 0))
                map[y][x] = TILE_PAREDE_INDESTRUTIVEL;
            else
                map[y][x] = (rand() % 100 < 30) ? TILE_PAREDE_DESTRUTIVEL : TILE_VAZIO;
        }
    }

    map[1][1] = TILE_VAZIO;
    map[1][2] = TILE_VAZIO;
    map[2][1] = TILE_VAZIO;

    int tentativas = 0;
    bool saidaColocada = false;

    while (!saidaColocada && tentativas < 1000)
    {
        int tempX = rand() % MAP_WIDTH;
        int tempY = rand() % MAP_HEIGHT;

        if (map[tempY][tempX] == TILE_PAREDE_DESTRUTIVEL &&
            (tempX > 3 || tempY > 3))
        {
            exitX = tempX;
            exitY = tempY;
            saidaColocada = true;
        }
        tentativas++;
    }

    if (!saidaColocada)
    {
        for (int y = MAP_HEIGHT - 2; y >= 1; y--)
        {
            for (int x = MAP_WIDTH - 2; x >= 1; x--)
            {
                if (map[y][x] == TILE_PAREDE_DESTRUTIVEL)
                {
                    exitX = x;
                    exitY = y;
                    saidaColocada = true;
                    break;
                }
            }
            if (saidaColocada)
                break;
        }
    }
}

void DrawLevel(
    Texture2D floorTexture,
    Texture2D destructibleTexture,
    Texture2D wallTexture,
    Texture2D exitTexture,
    Texture2D powerupBombTexture,
    Texture2D powerupRangeTexture)
{
    int offsetX = (GetScreenWidth() - (MAP_WIDTH * TILE_SIZE)) / 2;
    int offsetY = (GetScreenHeight() - (MAP_HEIGHT * TILE_SIZE)) / 2;

    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            Vector2 position = {
                (float)(offsetX + x * TILE_SIZE),
                (float)(offsetY + y * TILE_SIZE)
            };

            TileType currentTile = map[y][x];

            switch (currentTile)
            {
            case TILE_VAZIO:
                DrawTextureV(floorTexture, position, WHITE);
                break;

            case TILE_POWERUP_BOMBA:
                DrawTextureV(floorTexture, position, WHITE);
                DrawTextureV(powerupBombTexture, position, WHITE);
                break;

            case TILE_POWERUP_EXPLOSAO:
                DrawTextureV(floorTexture, position, WHITE);
                DrawTextureV(powerupRangeTexture, position, WHITE);
                break;

            case TILE_SAIDA:
                DrawTextureV(floorTexture, position, WHITE);
                DrawTextureV(exitTexture, position, WHITE);
                break;

            case TILE_PAREDE_DESTRUTIVEL:
                DrawTextureV(floorTexture, position, WHITE); // fundo transparente
                DrawTextureV(destructibleTexture, position, WHITE);
                break;

            case TILE_PAREDE_INDESTRUTIVEL:
                DrawTextureV(wallTexture, position, WHITE);
                break;
            }
        }
    }
}


bool CarregarMapaDeArquivo(const char *filename)
{
    FILE *f = fopen(filename, "r");
    if (!f)
        return false;

    char linha[MAP_WIDTH + 2];
    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        if (!fgets(linha, sizeof(linha), f))
            break;

        for (int x = 0; x < MAP_WIDTH && linha[x] != '\0'; x++)
        {
            if (linha[x] == 'W')
                map[y][x] = TILE_PAREDE_INDESTRUTIVEL;
            else if (linha[x] == 'B')
                map[y][x] = TILE_PAREDE_DESTRUTIVEL;
            else
                map[y][x] = TILE_VAZIO;
        }
    }

    fclose(f);
    return true;
}
