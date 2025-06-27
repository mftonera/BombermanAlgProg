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
    player.bombasDisponiveis = 10;
    player.bombasMaximas = 10;
    player.alcanceExplosao = 1;
    player.status = 1; // 1 = vivo, 0 = morto
    player.pontuacao = 0;
    player.direcaoSprite = -1; // -1 = direita, 1 = esquerda
}

void CheckPlayerDamage(void)
{
    if (player.status && IsExplosionAt(player.x, player.y)) // Se o jogador está vivo e há uma explosão na sua posição
    {
        player.status = 0; // O jogador morre :(
    }
}

void UpdatePlayer(Sound powerupSound)
{
    int newX = player.x;
    int newY = player.y;

    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)){ 
        newX++;
        player.direcaoSprite = -1; // Atualiza a direção do sprite para direita
    }
    if (IsKeyPressed(KEY_LEFT)  || IsKeyPressed(KEY_A)){
        newX--;
        player.direcaoSprite = 1; // Atualiza a direção do sprite para esquerda
    }
    if (IsKeyPressed(KEY_DOWN)  || IsKeyPressed(KEY_S)) newY++;
    if (IsKeyPressed(KEY_UP)    || IsKeyPressed(KEY_W)) newY--;

    // Verifica se a nova posição está dentro dos limites do mapa e se é um tile válido para o player estar
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
        player.bombasDisponiveis = player.bombasMaximas; // Recarreca todas as bombas do player
        map[player.y][player.x] = TILE_VAZIO;            // Altera o tile para vazio
        PlaySound(powerupSound);                         // Toca o som do powerup
    }
    else if (tile == TILE_POWERUP_EXPLOSAO)
    {
        player.alcanceExplosao++;               // Aumenta o alcance da explosão
        map[player.y][player.x] = TILE_VAZIO;   // Altera o tile para vazio
        PlaySound(powerupSound);                // Toca o som do powerup
    }

    CheckPlayerDamage();

    if (map[player.y][player.x] == TILE_SAIDA && IsKeyPressed(KEY_F)) // Se o jogador está na saída e pressionou F, passa/reinicia o nível
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

    Rectangle sourceRec = { 0.0f, 0.0f, (float)playerTexture.width, (float)playerTexture.height }; // Define a região da textura a ser desenhada (é usado por causa da alteração de "direção" do sprite)
    
    // Altera a direção do sprite com base na direção do player
    switch (player.direcaoSprite) {
        case -1: // Direita
        sourceRec.width = -sourceRec.width;
        break;

        case 1: // Esquerda
        sourceRec.width = sourceRec.width; 
        break;
    }

    // Define a posição de destino do player na tela
    Vector2 destPos = { 
        (float)(offsetX + player.x * TILE_SIZE), 
        (float)(offsetY + player.y * TILE_SIZE) 
    };

    DrawTextureRec(playerTexture, sourceRec, destPos, WHITE); // Desenha o player na tela com a textura e a direção correta
}
