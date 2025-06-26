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

Color fundoPersonalizado = (Color){ 28, 20, 41, 255 };

int main(void)
{
    InitWindow(800, 800, "Mini Bomberman");
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

    while (!WindowShouldClose())
    {
        while (GetKeyPressed() != 0)
            ;

        int choice = ShowMainMenu();
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
                UpdatePlayer();
            UpdateBombs();
            UpdateExplosions();
            UpdateEnemies();

            if (IsKeyPressed(KEY_ESCAPE))
            {
                int podeSalvar = (choice != 2);
                PauseAction acao = ShowPauseMenu(podeSalvar);

                if (acao == PAUSE_RETURN_TO_MENU)
                    break;
                if (acao == PAUSE_EXIT)
                {
                    CloseWindow();
                    return 0;
                }
                if (acao == PAUSE_SAVE && podeSalvar)
                {
                    SalvarJogo();
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
                powerupRangeTex      
            );

            DrawBombs(bombTexture);
            DrawExplosions();
            DrawEnemies(enemyTexture);

            int gridWidth = MAP_WIDTH * TILE_SIZE;
            int offsetX = (GetScreenWidth() - gridWidth) / 2;
            int offsetY = (GetScreenHeight() - (MAP_HEIGHT * TILE_SIZE)) / 2;

            char textoBombas[64];
            sprintf(textoBombas, "Bombas: %d / %d", player.bombasDisponiveis, player.bombasMaximas);
            DrawText(textoBombas, offsetX, offsetY - 50, 20, BLACK);

            char textoAlcance[64] = "Alcance: ";
            int baseFontSize = 20 + (player.alcanceExplosao - 1) * 2;
            char fogo[32] = "";
            for (int i = 0; i < player.alcanceExplosao; i++)
                strcat(fogo, "I");
            strcat(textoAlcance, fogo);
            int textWidth = MeasureText(textoAlcance, baseFontSize);
            DrawText(textoAlcance, offsetX + gridWidth - textWidth, offsetY - 50, baseFontSize, RED);

            char pontosTexto[64];
            sprintf(pontosTexto, "Pontos: %d", player.pontuacao);
            DrawText(pontosTexto, 10, 10, 20, BLACK);

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

    CloseWindow();
    return 0;
}