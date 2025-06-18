// src/player.c
#include "player.h"
#include "raylib.h"
#include "level.h"
#include "bomb.h"
#include "enemy.h"

Player player;
Texture2D playerTexture;

void InitPlayer(void)
{
    player.x = 1;
    player.y = 1;
    player.bombasDisponiveis = 50;
    player.bombasMaximas = 50;
    player.alcanceExplosao = 1;
    player.status = 1;
    player.pontuacao = 0;
    player.facingDirection = -1;
}

void CheckPlayerDamage(void)
{
    if (player.status && IsExplosionAt(player.x, player.y))
    {
        player.status = 0;
    }
}

void UpdatePlayer(void)
{
    int newX = player.x;
    int newY = player.y;

    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)){ 
        newX++;
        player.facingDirection = -1;
    }
    if (IsKeyPressed(KEY_LEFT)  || IsKeyPressed(KEY_A)){
        newX--;
        player.facingDirection = 1;
    }
    if (IsKeyPressed(KEY_DOWN)  || IsKeyPressed(KEY_S)) newY++;
    if (IsKeyPressed(KEY_UP)    || IsKeyPressed(KEY_W)) newY--;

    if (newX >= 0 && newX < MAP_WIDTH && newY >= 0 && newY < MAP_HEIGHT)
    {
        if (map[newY][newX] == TILE_VAZIO ||
            map[newY][newX] == TILE_POWERUP_BOMBA ||
            map[newY][newX] == TILE_POWERUP_EXPLOSAO ||
            map[newY][newX] == TILE_SAIDA)
        {
            player.x = newX;
            player.y = newY;
        }
    }

    TileType tile = map[player.y][player.x];

    if (tile == TILE_POWERUP_BOMBA)
    {
        if (player.bombasMaximas < MAX_BOMBS)
            player.bombasMaximas++;
        player.bombasDisponiveis = player.bombasMaximas;
        map[player.y][player.x] = TILE_VAZIO;
    }
    else if (tile == TILE_POWERUP_EXPLOSAO)
    {
        player.alcanceExplosao++;
        map[player.y][player.x] = TILE_VAZIO;
    }

    CheckPlayerDamage();

    if (map[player.y][player.x] == TILE_SAIDA && IsKeyPressed(KEY_F))
    {
        int currentScore = player.pontuacao;
        InitLevel();
        InitPlayer();
        player.pontuacao = currentScore;
        InitBombs();
        InitEnemies();
    }
}

void DrawPlayer(Texture2D playerTexture)
{
    int offsetX = (GetScreenWidth() - (MAP_WIDTH * TILE_SIZE)) / 2;
    int offsetY = (GetScreenHeight() - (MAP_HEIGHT * TILE_SIZE)) / 2;

    Rectangle sourceRec = { 0.0f, 0.0f, (float)playerTexture.width, (float)playerTexture.height };
    
    if (player.facingDirection == -1) {
        sourceRec.width = -sourceRec.width;
    }

    Vector2 destPos = { 
        (float)(offsetX + player.x * TILE_SIZE), 
        (float)(offsetY + player.y * TILE_SIZE) 
    };

    DrawTextureRec(playerTexture, sourceRec, destPos, WHITE);
}
