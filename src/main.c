#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"
#include "menu.h"
#include "level.h"
#include "player.h"
#include "bomb.h"
#include "enemy.h"
#include "save.h"
#include "editor.h"
#include "pause.h"
#include "map_selector.h"

Color fundoPersonalizado = (Color){28, 20, 41, 255};

int main(void)
{
    InitWindow(800, 800, "Mini Bomberman");
    InitAudioDevice();
    SetExitKey(KEY_NINE);
    SetTargetFPS(60);

    Font customFont = LoadFont("resources/fonte.ttf");

    Texture2D playerTexture = LoadTexture("resources/player.png");
    Texture2D enemyTexture = LoadTexture("resources/enemy.png");
    Texture2D bombTexture = LoadTexture("resources/bomb.png");
    Texture2D groundTex = LoadTexture("resources/tiles.png");
    Texture2D wallPlain = LoadTexture("resources/wall_plain.png");
    Texture2D destructibleTex = LoadTexture("resources/destructible.png");
    Texture2D exitTex = LoadTexture("resources/exit.png");
    Texture2D powerupBombTex = LoadTexture("resources/powerup_bomb.png");
    Texture2D powerupRangeTex = LoadTexture("resources/powerup_range.png");
    Texture2D titleTexture = LoadTexture("resources/titulo.png");

    Sound powerupSound = LoadSound("resources/powerup.wav");
    Sound explosionSound = LoadSound("resources/explosion.wav");

    while (!WindowShouldClose())
    {
        while (GetKeyPressed() != 0)
            ;
        ;

        int choice = ShowMainMenu(customFont, titleTexture, fundoPersonalizado, playerTexture);

        if (choice == 4)
            break;

        if (choice == 0)
        {
            InitLevel();
            InitPlayer();
            InitBombs();
            InitEnemies();
        }
        else if (choice == 1)
        {
            if (!CarregarJogo())
            {
                InitLevel();
                InitPlayer();
                InitBombs();
                InitEnemies();
            }
        }
        else if (choice == 2)
        {
            char *path = SelecionarMapaPersonalizado();
            if (path != NULL)
            {
                if (!CarregarMapaDeArquivo(path))
                {
                    InitLevel();
                }
                free(path);
                InitPlayer();
                InitBombs();
                InitEnemies();
            }
            else
            {
                continue;
            }
        }
        else if (choice == 3)
        {
            while (GetKeyPressed() != 0)
                ;
            AbrirEditorDeMapa();
            continue;
        }

        while (!WindowShouldClose())
        {
            if (player.status)
            UpdatePlayer(powerupSound);
            UpdateBombs(explosionSound);
            UpdateExplosions();
            UpdateEnemies();

            if (IsKeyPressed(KEY_ESCAPE))
            {
                printf("[DEBUG] ESC pressionado. Exibindo menu de pausa...\n");

                PauseAction acao = ShowPauseMenu(choice != 2, customFont); // TRUE = jogo, FALSE = mapa personalizado

                printf("[DEBUG] Acao do menu de pausa retornada: %d\n", acao);

                if (acao == PAUSE_RETURN)
                {
                    printf("[DEBUG] Ação: RETURN - continuando o jogo normalmente.\n");
                    continue;
                }
                else if (acao == PAUSE_RETURN_TO_MENU)
                {
                    printf("[DEBUG] Ação: RETURN_TO_MENU - quebrando loop do jogo para voltar ao menu principal.\n");
                    break; // volta para o menu principal
                }
                else if (acao == PAUSE_EXIT)
                {
                    printf("[DEBUG] Ação: EXIT - encerrando ou saindo do editor.\n");
                    if (choice == 2)
                    {
                        printf("[DEBUG] Modo mapa personalizado detectado. Quebrando para voltar à seleção de mapa.\n");
                        break;
                    }
                    else
                    {
                        printf("[DEBUG] Encerrando o jogo com CloseWindow().\n");
                        CloseWindow();
                        return 0;
                    }
                }
                else if (acao == PAUSE_SAVE && choice != 2)
                {
                    printf("[DEBUG] Ação: SAVE - salvando o jogo.\n");
                    SalvarJogo();
                }
                else
                {
                    printf("[DEBUG] Ação desconhecida ou inválida recebida do menu de pausa: %d\n", acao);
                }
            }

            BeginDrawing();
            ClearBackground(fundoPersonalizado);

            DrawLevel(
                groundTex,
                destructibleTex,
                wallPlain,
                exitTex,
                powerupBombTex,
                powerupRangeTex);

            DrawBombs(bombTexture);
            DrawExplosions();
            DrawEnemies(enemyTexture);

            int gridWidth = MAP_WIDTH * TILE_SIZE;
            int offsetX = (GetScreenWidth() - gridWidth) / 2;
            int offsetY = (GetScreenHeight() - (MAP_HEIGHT * TILE_SIZE)) / 2;
            float hudFontSize = 30;
            float spacing = 1;

            // Bombas
            char textoBombas[64];
            sprintf(textoBombas, "Bombas: %d / %d", player.bombasDisponiveis, player.bombasMaximas);
            DrawTextEx(customFont, textoBombas, (Vector2){offsetX, offsetY - 30}, hudFontSize, spacing, WHITE);

            // Alcance
            char textoAlcance[64];
            sprintf(textoAlcance, "Alcance: %d", player.alcanceExplosao);
            DrawTextEx(customFont, textoAlcance, (Vector2){offsetX + gridWidth - 120, offsetY - 30}, hudFontSize, spacing, WHITE);

            // Pontos
            char textoPontos[64];
            sprintf(textoPontos, "Pontos: %d", player.pontuacao);
            DrawTextEx(customFont, textoPontos, (Vector2){10, 10}, hudFontSize, spacing, WHITE);

            if (player.status)
            {
                DrawPlayer(playerTexture);
            }
            else
            {
                DrawText("GAME  OVER", 250, 220, 40, RED);
                DrawText("Pressione R para reiniciar ou ESC para sair", 120, 280, 20, DARKGRAY);

                if (IsKeyPressed(KEY_R))
                {
                    InitLevel();
                    InitPlayer();
                    InitBombs();
                    InitEnemies();
                    while (GetKeyPressed() != 0)
                        ;
                    continue;
                }

                if (IsKeyPressed(KEY_ESCAPE))
                    break;
            }

            EndDrawing();
        }
        choice = -1;
    }

    UnloadTexture(playerTexture);
    UnloadTexture(enemyTexture);
    UnloadTexture(bombTexture);
    UnloadTexture(groundTex);
    UnloadTexture(destructibleTex);
    UnloadTexture(wallPlain);
    UnloadTexture(exitTex);
    UnloadTexture(powerupBombTex);
    UnloadTexture(powerupRangeTex);
    UnloadTexture(titleTexture);

    UnloadSound(powerupSound);
    UnloadSound(explosionSound);

    CloseAudioDevice();
    CloseWindow();
    return 0;
}