#include "level.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

TileType map[MAP_HEIGHT][MAP_WIDTH];
int exitX, exitY;

void InitLevel(void)
{
    srand(time(NULL)); // Gerador de números aleatórios

    // Inicializa a logica de geração do mapa
    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            if (y == 0 || y == MAP_HEIGHT - 1 || x == 0 || x == MAP_WIDTH - 1 || (y % 2 == 0 && x % 2 == 0)) // Se o tile X,Y for em uma extremidade ou combinação de numeros pares
                map[y][x] = TILE_PAREDE_INDESTRUTIVEL;                                                       // Coloca uma parede indestrutível
            else
                map[y][x] = (rand() % 100 < 30) ? TILE_PAREDE_DESTRUTIVEL : TILE_VAZIO; // Se não se enquadrar na opção acima, a tile terá 30% de chance de ser uma parede destrutível, senão é vazio
        }
    }

    // Garante que os tiles "iniciais" estejam vazios
    map[1][1] = TILE_VAZIO;
    map[1][2] = TILE_VAZIO;
    map[2][1] = TILE_VAZIO;

    int tentativas = 0;
    bool saidaColocada = false; // Flag para verificar se a saída foi colocada

    while (!saidaColocada && tentativas < 1000)
    {
        int tempX = rand() % MAP_WIDTH;  // Gera um número aleatório para a posição X
        int tempY = rand() % MAP_HEIGHT; // Gera um número aleatório para a posição Y

        if (map[tempX][tempY] == TILE_PAREDE_DESTRUTIVEL && // Verifica se o tile é uma parede destrutível
            (tempX > 3 || tempY > 3))                       // Garante que a saída não esteja muito próxima do local de início
        {
            // Se satisfazer as condições, coloca a saída
            exitX = tempX;
            exitY = tempY;
            saidaColocada = true; // Marca que a saída foi colocada
        }
        tentativas++; // Incrementa o número de tentativas (programa tenta 1000 vezes encontrar um local adequado para a saída) acho que é o suficiente KKKKKK
    }

    // Se não conseguiu colocar a saída em 1000 tentativas, tenta colocar em um local específico (varre o mapa de baixo para cima e da direita para a esquerda até achar uma tile destrutível)
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

// Função do raylib "DrawLevel" recebe os parametros de todas texturas necessarias para desenhar o mapa
void DrawLevel(
    Texture2D floorTexture,
    Texture2D destructibleTexture,
    Texture2D wallTexture,
    Texture2D exitTexture,
    Texture2D powerupBombTexture,
    Texture2D powerupRangeTexture)
{
    // Calcula o offset para centralizar o mapa na tela
    int offsetX = (GetScreenWidth() - (MAP_WIDTH * TILE_SIZE)) / 2;
    int offsetY = (GetScreenHeight() - (MAP_HEIGHT * TILE_SIZE)) / 2;

    // Percorre a matriz do mapa e desenha os tiles de acordo com o tipo
    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        for (int x = 0; x < MAP_WIDTH; x++)
        {
            // Calcula a posição do tile na tela
            // offsetX e offsetY são usados para centralizar o mapa na tela
            Vector2 position = {
                (float)(offsetX + x * TILE_SIZE),
                (float)(offsetY + y * TILE_SIZE)};

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
                DrawTextureV(floorTexture, position, WHITE);
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

    char linha[MAP_WIDTH + 2]; // +2 para '\n' e '\0'

    for (int y = 0; y < MAP_HEIGHT; y++)
    {
        if (!fgets(linha, sizeof(linha), f)) // Lê uma linha do arquivo
            break;

        for (int x = 0; x < MAP_WIDTH && linha[x] != '\0'; x++) // Percorre cada caractere da linha
        {
            if (linha[x] == 'W') // W para parede indestrutível
                map[y][x] = TILE_PAREDE_INDESTRUTIVEL;
            else if (linha[x] == 'B') // B para parede destrutível
                map[y][x] = TILE_PAREDE_DESTRUTIVEL;
            else
                map[y][x] = TILE_VAZIO; // Qualquer outro caractere é considerado vazio
        }
    }

    fclose(f);
    return true;
}