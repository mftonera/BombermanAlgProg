#include "enemy.h"
#include "level.h"
#include "bomb.h"
#include "player.h"
#include "raylib.h"
#include <stdlib.h>
#include <time.h>

Enemy enemies[MAX_ENEMIES];
Texture2D enemyTexture;

bool IsEnemyAt(int x, int y, int ignoreIndex)
{
    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        if (i == ignoreIndex || !enemies[i].alive) continue;
        if (enemies[i].x == x && enemies[i].y == y) return true;
    }
    return false;
}

void InitEnemies(void)
{
    static bool texturaCarregada = false;
    if (!texturaCarregada)
    {
        enemyTexture = LoadTexture("resources/enemy.png");
        texturaCarregada = true;
    }

    static bool sementeInicializada = false;
    if (!sementeInicializada)
    {
        srand(time(NULL));
        sementeInicializada = true;
    }

    for (int i = 0; i < MAX_ENEMIES; i++)
    {
        int x, y, tentativas = 0;

        do
        {
            x = rand() % MAP_WIDTH;
            y = rand() % MAP_HEIGHT;
            tentativas++;

            bool ocupado = false;
            for (int j = 0; j < i; j++)
            {
                if (enemies[j].x == x && enemies[j].y == y)
                {
                    ocupado = true;
                    break;
                }
            }

            if (map[y][x] == TILE_VAZIO && !ocupado && (x > 3 || y > 3))
                break;

        } while (tentativas < 100);

        enemies[i].x = x;
        enemies[i].y = y;

        int d = rand() % 4;
        enemies[i].dirX = (d == 0) - (d == 1);
        enemies[i].dirY = (d == 2) - (d == 3);

        enemies[i].alive = 1;
        enemies[i].moveTimer = 0;
        enemies[i].facingDirection = 1;
    }
}

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
            enemies[i].facingDirection = enemies[i].dirX;
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
        
        if (enemies[i].facingDirection == 1) {
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
