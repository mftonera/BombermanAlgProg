// src/bomb.c
#include "bomb.h"
#include "player.h"
#include "level.h"
#include "raylib.h"
#include <stdio.h>  
#include <stdlib.h>

Bomb bombs[MAX_BOMBS];
Explosion explosions[MAX_EXPLOSIONS];
Texture2D bombTexture;

// Para reação em cadeia
int bombasParaDetonar[MAX_BOMBS];
int numBombasParaDetonar = 0;

// Inicializa/reseta o estado das bombas
void InitBombs(void)
{
    for (int i = 0; i < MAX_BOMBS; i++)
    {
        bombs[i].active = 0;
    }
    numBombasParaDetonar = 0;
}

// Planta uma bomba na posição (x, y) do jogador
void PlantBomb(int x, int y)
{
    for (int i = 0; i < MAX_BOMBS; i++) // Percorre todas as bombas até encontrar uma inativa
    {
        if (!bombs[i].active)
        {
            bombs[i].x = x;
            bombs[i].y = y;
            bombs[i].timer = 2.0f;
            bombs[i].active = 1;
            break;
        }
    }
}

// Cria a explosão de uma bomba na posição (x, y)
void ExplodeBomb(int x, int y)
{
    int alcance = player.alcanceExplosao;

    AddExplosion(x, y);

    // Direções: direita, esquerda, baixo, cima
    int dx[] = {1, -1, 0, 0}; 
    int dy[] = {0, 0, 1, -1}; 

    // Loop para cada direção
    for (int dir = 0; dir < 4; dir++)
    {
        for (int i = 1; i <= alcance; i++)
        {
            int nx = x + dx[dir] * i;
            int ny = y + dy[dir] * i;

            if (nx < 0 || ny < 0 || nx >= MAP_WIDTH || ny >= MAP_HEIGHT) // Explosão para de se propagar se sair dos limites do mapa
                break;

            if (map[ny][nx] == TILE_PAREDE_INDESTRUTIVEL) // Se encontrar uma parede indestrutível, a explosão para
            {
                break;
            }

            DetonateBombAt(nx, ny);
            AddExplosion(nx, ny);

            if (map[ny][nx] == TILE_PAREDE_DESTRUTIVEL) // Verifica se é uma parede destrutivel
            {
                if (nx == exitX && ny == exitY) // Verifica se é a saída
                {
                    map[ny][nx] = TILE_SAIDA; // Coloca a saída no lugar da parede destruída
                }
                else
                {
                    int sorteio = rand() % 100; // Sorteia um número de 0 a 99 para decidir o que colocar no lugar da parede destruída
                    if (sorteio < 20)
                        map[ny][nx] = TILE_POWERUP_BOMBA; // 20% de chance de ser um powerup de bomba
                    else if (sorteio < 40)
                        map[ny][nx] = TILE_POWERUP_EXPLOSAO; // 20% de chance de ser um powerup de alcance de explosão
                    else
                        map[ny][nx] = TILE_VAZIO; // 60% de chance de ser um tile vazio
                }
                break;
            }
            else if (map[ny][nx] == TILE_SAIDA && nx != x && ny != y)
            {
                break;
            }
        }
    }

    if (x == exitX && y == exitY)
    {
        map[y][x] = TILE_SAIDA;
    }
    else
    {
        map[y][x] = TILE_VAZIO;
    }
}

// Cria tiles de explosão na posição (x, y)
void AddExplosion(int x, int y)
{
    for (int i = 0; i < MAX_EXPLOSIONS; i++)
    {
        if (!explosions[i].active)
        {
            explosions[i].x = x;
            explosions[i].y = y;
            explosions[i].timer = 0.8f; // Duração da explosão
            explosions[i].active = 1;
            break;
        }
    }
}

// Atualiza o estado das bombas, verifica se elas devem explodir e executa as explosões
void UpdateBombs(Sound boom)
{
    for (int i = 0; i < MAX_BOMBS; i++) 
    {
        if (bombs[i].active)
        {
            bombs[i].timer -= GetFrameTime(); 
            if (bombs[i].timer <= 0) // Se o timer da bomba chegou a zero ela explode!!!
            {
                ExplodeBomb(bombs[i].x, bombs[i].y);
                bombs[i].active = 0;
                PlaySound(boom); // Toca o som de BOOOOM
            }
        }
    }

    // Executar bombas marcadas para reação em cadeia
    for (int i = 0; i < numBombasParaDetonar; i++)
    {
        int idx = bombasParaDetonar[i];
        if (bombs[idx].active)
        {
            ExplodeBomb(bombs[idx].x, bombs[idx].y);
            bombs[idx].active = 0;
            //Aqui não toca o som de explosão novamente, se não seria um som para cada bomba detonada
        }
    }
    numBombasParaDetonar = 0;

    // Verifica se o jogador pressionou a tecla de bomba e se ainda tem bombas disponíveis
    if (IsKeyPressed(KEY_SPACE) && player.bombasDisponiveis > 0)
    {
        PlantBomb(player.x, player.y); // Planta uma bomba na posição do jogador
        player.bombasDisponiveis--; // Decrementa o número de bombas disponíveis
    }
}

void DrawBombs(Texture2D bombTexture)
{
    int offsetX = (GetScreenWidth() - (MAP_WIDTH * TILE_SIZE)) / 2;
    int offsetY = (GetScreenHeight() - (MAP_HEIGHT * TILE_SIZE)) / 2;

    for (int i = 0; i < MAX_BOMBS; i++)
    {
        if (bombs[i].active)
        {
            DrawTexture(
                bombTexture,
                offsetX + bombs[i].x * TILE_SIZE,
                offsetY + bombs[i].y * TILE_SIZE,
                WHITE);
        }
    }
}

// Atualiza o estado das explosões, decrementando o timer e desativando as explosões que já passaram do tempo
void UpdateExplosions(void)
{
    for (int i = 0; i < MAX_EXPLOSIONS; i++)
    {
        if (explosions[i].active)
        {
            explosions[i].timer -= GetFrameTime();
            if (explosions[i].timer <= 0)
            {
                explosions[i].active = 0; // "Limpa" a explosão
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
                ORANGE); // Laranja pq é FOGOOOO!!!
        }
    }
}

// Verifica se há uma explosão na posição (x, y) 
// É com ela que o inimigo morre :) e o player perde vida :(
int IsExplosionAt(int x, int y)
{
    for (int i = 0; i < MAX_EXPLOSIONS; i++) // Percorre a array de explosões
    {
        if (explosions[i].active && explosions[i].x == x && explosions[i].y == y) // Verifica se a explosão tá ativa na posição (x, y)
        {
            return 1; // Retorna 1 se houver uma explosão na posição (x, y)
        }
    }
    return 0; // Retorna 0 se não houver explosão na posição (x, y)
}

// Marca bombas para detonar em cadeia
// Essa função é chamada quando uma bomba explode e verifica se há outras bombas por perto para explodir em sequência
void DetonateBombAt(int x, int y)
{
    // Inicia um loop para percorrer todas as bombas do jogo
    for (int i = 0; i < MAX_BOMBS; i++)
    {
        // Verifica se há uma bomba ativa na posição (x, y)
        if (bombs[i].active && bombs[i].x == x && bombs[i].y == y)
        {
            int marcada = 0; // Flag para indicar se a bomba já está na lista
            for (int j = 0; j < numBombasParaDetonar; j++)
            {
                if (bombasParaDetonar[j] == i) // Verifica se o índice da bomba (i) já está na lista 'bombasParaDetonar'
                {
                    marcada = 1; // Se já estiver na lista, marca como 1
                    break;
                }
            }

            if (!marcada && numBombasParaDetonar < MAX_BOMBS) // Se a bomba ainda não foi marcada && a lista de detonação não está cheia
            {
                bombasParaDetonar[numBombasParaDetonar++] = i; // Adiciona o índice da bomba à lista de bombas para detonar
            }
            break;
        }
    }
}

// Verifica se há uma bomba na posição (x, y)
int IsBombAt(int x, int y)
{
    for (int i = 0; i < MAX_BOMBS; i++)
    {
        if (bombs[i].active && bombs[i].x == x && bombs[i].y == y)
        {
            return 1;
        }
    }
    return 0;
}
