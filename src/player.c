// src/player.c
#include "player.h"
#include "raylib.h"
#include "level.h"
#include "bomb.h"
#include "enemy.h" // Adicionado para reiniciar inimigos

Player player;

void InitPlayer(void)
{
    player.x = 1;
    player.y = 1;
    player.bombasDisponiveis = 50;
    player.bombasMaximas = 50;
    player.alcanceExplosao = 1;
    player.status = 1;
    player.pontuacao = 0; // A pontuação será mantida ao reiniciar a fase.
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
        map[newY][newX] == TILE_SAIDA) // O jogador pode andar sobre TILE_SAIDA
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

    // --- Nova parte: Lógica de passagem de fase ---
    if (map[player.y][player.x] == TILE_SAIDA && IsKeyPressed(KEY_F)) {
        // Salva a pontuação atual antes de reiniciar
        int currentScore = player.pontuacao;

        // Reinicia o nível
        InitLevel();
        InitPlayer(); // Reinicia posição e status do jogador, bombas, etc.

        // Restaura a pontuação
        player.pontuacao = currentScore;

        // Reinicia bombas e explosões
        InitBombs();
        // Reinicia inimigos para a nova fase
        InitEnemies();
    }
    // --- Fim da nova parte ---
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