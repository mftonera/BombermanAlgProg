// src/player.c
#include "player.h"
#include "raylib.h"
#include "level.h"
#include "bomb.h"

Player player;

void InitPlayer(void)
{
    player.x = 1;
    player.y = 1;
    player.bombasDisponiveis = 5;
    player.bombasMaximas = 5;
    player.alcanceExplosao = 1;
    player.status = 1;
    player.pontuacao = 0;
}

void CheckPlayerDamage(void)
{
    if (player.status && IsExplosionAt(player.x, player.y))
    {
        player.status = 0; // Morreu
    }
}

void UpdatePlayer(void)
{
    int newX = player.x;
    int newY = player.y;

    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D))
        newX++;
    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A))
        newX--;
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S))
        newY++;
    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W))
        newY--;

    // Verificar colisão com o mapa
    if (map[newY][newX] == TILE_VAZIO ||
        map[newY][newX] == TILE_POWERUP_BOMBA ||
        map[newY][newX] == TILE_POWERUP_EXPLOSAO ||
        map[newY][newX] == TILE_SAIDA)
    {
        player.x = newX;
        player.y = newY;
    }

    TileType tile = map[player.y][player.x];

    if (tile == TILE_POWERUP_BOMBA)
    {
        if (player.bombasMaximas < MAX_BOMBS)
            player.bombasMaximas++;
        player.bombasDisponiveis = player.bombasMaximas; // recarrega
        map[player.y][player.x] = TILE_VAZIO;
    }
    else if (tile == TILE_POWERUP_EXPLOSAO)
    {
        player.alcanceExplosao++;
        map[player.y][player.x] = TILE_VAZIO;
    }

    CheckPlayerDamage();
}

void DrawPlayer(void)
{
    int offsetX = (GetScreenWidth() - (MAP_WIDTH * TILE_SIZE)) / 2;
    int offsetY = (GetScreenHeight() - (MAP_HEIGHT * TILE_SIZE)) / 2;

    DrawRectangle(
        offsetX + player.x * TILE_SIZE,
        offsetY + player.y * TILE_SIZE,
        TILE_SIZE,
        TILE_SIZE,
        BLUE);
}
