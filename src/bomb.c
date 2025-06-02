// src/bomb.c
#include "bomb.h"
#include "player.h"
#include "level.h"
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

Bomb bombs[MAX_BOMBS];
static Explosion explosions[MAX_EXPLOSIONS];

void InitBombs(void)
{
    for (int i = 0; i < MAX_BOMBS; i++)
    {
        bombs[i].active = 0;
    }
}

void PlantBomb(int x, int y)
{
    for (int i = 0; i < MAX_BOMBS; i++)
    {
        if (!bombs[i].active)
        {
            bombs[i].x = x;
            bombs[i].y = y;
            bombs[i].timer = 2.0f; // 2 segundos
            bombs[i].active = 1;
            break;
        }
    }
}

void ExplodeBomb(int x, int y)
{
    int alcance = player.alcanceExplosao;

    AddExplosion(x, y);
    map[y][x] = TILE_VAZIO;

    int dx[] = {1, -1, 0, 0};
    int dy[] = {0, 0, 1, -1};

    for (int dir = 0; dir < 4; dir++)
    {
        for (int i = 1; i <= alcance; i++)
        {
            int nx = x + dx[dir] * i;
            int ny = y + dy[dir] * i;

            if (nx < 0 || ny < 0 || nx >= MAP_WIDTH || ny >= MAP_HEIGHT)
                break;

            DetonateBombAt(nx, ny);

            if (map[ny][nx] == TILE_PAREDE_INDESTRUTIVEL)
                break;

            AddExplosion(nx, ny);

            if (map[ny][nx] == TILE_PAREDE_DESTRUTIVEL)
            {
                int sorteio = rand() % 100;
                if (sorteio < 20)
                {
                    map[ny][nx] = TILE_POWERUP_BOMBA;
                }
                else if (sorteio < 40)
                {
                    map[ny][nx] = TILE_POWERUP_EXPLOSAO;
                }
                else
                {
                    map[ny][nx] = TILE_VAZIO;
                }
                break;
            }
        }
    }
}

void UpdateBombs(void)
{
    for (int i = 0; i < MAX_BOMBS; i++)
    {
        if (bombs[i].active)
        {
            bombs[i].timer -= GetFrameTime();
            if (bombs[i].timer <= 0)
            {
                ExplodeBomb(bombs[i].x, bombs[i].y);
                bombs[i].active = 0;
            }
        }
    }

    // Plantar bomba com espaço
    if ((IsKeyPressed(KEY_SPACE)) && player.bombasDisponiveis > 0)
    {
        PlantBomb(player.x, player.y);
        player.bombasDisponiveis--;
    }
}

void DrawBombs(void)
{
    int offsetX = (GetScreenWidth() - (MAP_WIDTH * TILE_SIZE)) / 2;
    int offsetY = (GetScreenHeight() - (MAP_HEIGHT * TILE_SIZE)) / 2;

    for (int i = 0; i < MAX_BOMBS; i++)
    {
        if (bombs[i].active)
        {
            DrawCircle(
                offsetX + bombs[i].x * TILE_SIZE + TILE_SIZE / 2,
                offsetY + bombs[i].y * TILE_SIZE + TILE_SIZE / 2,
                TILE_SIZE / 3,
                BLACK);
        }
    }
}

void AddExplosion(int x, int y)
{
    for (int i = 0; i < MAX_EXPLOSIONS; i++)
    {
        if (!explosions[i].active)
        {
            explosions[i].x = x;
            explosions[i].y = y;
            explosions[i].timer = 0.5f;
            explosions[i].active = 1;
            break;
        }
    }
}

void UpdateExplosions(void)
{
    for (int i = 0; i < MAX_EXPLOSIONS; i++)
    {
        if (explosions[i].active)
        {
            explosions[i].timer -= GetFrameTime();
            if (explosions[i].timer <= 0)
            {
                explosions[i].active = 0;
            }
        }
    }
}

void DrawExplosions(void)
{
    int offsetX = (GetScreenWidth() - (MAP_WIDTH * TILE_SIZE)) / 2;
    int offsetY = (GetScreenHeight() - (MAP_HEIGHT * TILE_SIZE)) / 2;

    for (int i = 0; i < MAX_EXPLOSIONS; i++)
    {
        if (explosions[i].active)
        {
            DrawRectangle(
                offsetX + explosions[i].x * TILE_SIZE,
                offsetY + explosions[i].y * TILE_SIZE,
                TILE_SIZE,
                TILE_SIZE,
                ORANGE);
        }
    }
}

int IsExplosionAt(int x, int y)
{
    for (int i = 0; i < MAX_EXPLOSIONS; i++)
    {
        if (explosions[i].active && explosions[i].x == x && explosions[i].y == y)
        {
            return 1;
        }
    }
    return 0;
}

void DetonateBombAt(int x, int y)
{
    for (int i = 0; i < MAX_BOMBS; i++)
    {
        if (bombs[i].active && bombs[i].x == x && bombs[i].y == y)
        {
            ExplodeBomb(x, y);
            bombs[i].active = 0;
            break; // Evita múltiplas ativações da mesma bomba
        }
    }
}
