#include "enemy.h"
#include "level.h"
#include "bomb.h"
#include "player.h"
#include "raylib.h"
#include <stdlib.h>
#include <time.h>

Enemy enemies[MAX_ENEMIES];
Texture2D enemyTexture;

// Verifica se há um inimigo na posição (x, y), ignorando o índice do inimigo atual
bool IsEnemyAt(int x, int y, int ignoreIndex)
{
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        if (i == ignoreIndex || !enemies[i].alive) continue;
        if (enemies[i].x == x && enemies[i].y == y) return true;
    }
    return false;
}

// Inicializa os inimigos, carregando a textura e posicionando-os aleatoriamente no mapa
void InitEnemies(void)
{
    static bool texturaCarregada = false; // 'static bool' faz com que a variável mantenha seu valor entre as chamadas da função (ai não precisa carregar a textura toda vez que a função é chamada)
    if (!texturaCarregada)
    {
        enemyTexture = LoadTexture("resources/enemy.png");
        texturaCarregada = true;
    }

    // Garante que a semente do gerador de números aleatórios seja inicializada apenas uma vez
    static bool sementeInicializada = false;
    if (!sementeInicializada)
    {
        srand(time(NULL));
        sementeInicializada = true;
    }

    // Loop de inicialização dos inimigos
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        int x, y, tentativas = 0;

        do
        {
            x = rand() % MAP_WIDTH;
            y = rand() % MAP_HEIGHT;
            tentativas++;

            // Verifica se a posição (x, y) já está ocupada por outro inimigo
            bool ocupado = false;
            for (int j = 0; j < i; j++)
            {
                if (enemies[j].x == x && enemies[j].y == y)
                {
                    ocupado = true;
                    break;
                }
            }

            // Se a posição é vazia e não está ocupada por outro inimigo, e não é muito próxima do início (x > 3 ou y > 3), então é uma posição válida
            if (map[y][x] == TILE_VAZIO && !ocupado && (x > 3 || y > 3))
                break;

        } while (tentativas < 100); // Tenta até 100 vezes encontrar uma posição válida

        // Define a posição do inimigo
        enemies[i].x = x;
        enemies[i].y = y;

        int d = rand() % 4;
        enemies[i].dirX = (d == 0) - (d == 1);
        enemies[i].dirY = (d == 2) - (d == 3);

        // Inicializa as propriedades do inimigo
        enemies[i].alive = 1;
        enemies[i].moveTimer = 0;
        enemies[i].direcaoSprite = 1;
    }
}

// Atualiza a lógica dos inimigos
void UpdateEnemies(void)
{
    float moveDelay = 0.8f; // segundos entre cada passo

    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        if (!enemies[i].alive)
            continue;

        enemies[i].moveTimer += GetFrameTime();
        if (enemies[i].moveTimer < moveDelay)
            continue; // ainda não é hora de mover

        enemies[i].moveTimer = 0; // resetar timer após mover

        int nx = enemies[i].x + enemies[i].dirX;
        int ny = enemies[i].y + enemies[i].dirY;

        bool bateu = (nx < 0 || ny < 0 || nx >= MAP_WIDTH || ny >= MAP_HEIGHT ||
                      map[ny][nx] == TILE_PAREDE_INDESTRUTIVEL ||
                      map[ny][nx] == TILE_PAREDE_DESTRUTIVEL ||
                      IsEnemyAt(nx, ny, i)
        );

        if (bateu)
        {
            // Tentar uma nova direção aleatória até encontrar uma válida
            for (int tentativa = 0; tentativa < 4; tentativa++)
            {
                int d = rand() % 4;
                int dx = (d == 0) - (d == 1); // 1 ou -1 para X
                int dy = (d == 2) - (d == 3); // 1 ou -1 para Y

                int tx = enemies[i].x + dx;
                int ty = enemies[i].y + dy;

                if (tx >= 0 && tx < MAP_WIDTH &&
                    ty >= 0 && ty < MAP_HEIGHT &&
                    map[ty][tx] == TILE_VAZIO)
                {
                    enemies[i].dirX = dx;
                    enemies[i].dirY = dy;
                    break;
                }
            }
        }
        else
        {
            enemies[i].x = nx;
            enemies[i].y = ny;
        }

        // Atualiza a direção que o inimigo "olha" com base no movimento horizontal
        if (enemies[i].dirX != 0)
        {
            enemies[i].direcaoSprite = enemies[i].dirX;
        }

        // Checar dano por fogo
        if (IsExplosionAt(enemies[i].x, enemies[i].y))
        {
            enemies[i].alive = 0;
            player.pontuacao += 100;
        }

        // Checar colisão com jogador
        if (player.status && enemies[i].x == player.x && enemies[i].y == player.y)
        {
            player.status = 0;
        }
    }
}

void DrawEnemies(Texture2D enemyTexture)
{
    int offsetX = (GetScreenWidth() - (MAP_WIDTH * TILE_SIZE)) / 2;
    int offsetY = (GetScreenHeight() - (MAP_HEIGHT * TILE_SIZE)) / 2;

    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        if (!enemies[i].alive)
            continue;

        Rectangle sourceRec = { 0.0f, 0.0f, (float)enemyTexture.width, (float)enemyTexture.height };
        
        if (enemies[i].direcaoSprite == 1) {
            sourceRec.width = -sourceRec.width;
        }

        Vector2 destPos = { 
            (float)(offsetX + enemies[i].x * TILE_SIZE), 
            (float)(offsetY + enemies[i].y * TILE_SIZE) 
        };

        DrawTextureRec(enemyTexture, sourceRec, destPos, WHITE);
    }
}

void UnloadEnemies(void)
{
    static bool descarregado = false;
    if (!descarregado)
    {
        UnloadTexture(enemyTexture);
        descarregado = true;
    }
}
