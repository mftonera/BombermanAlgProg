#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"
#include "menu.h"
#include "level.h"
#include "player.h"
#include "bomb.h"
#include "enemy.h"

int main(void)
{
    InitWindow(800, 800, "Mini Bomberman");
    SetTargetFPS(60);

    int choice = ShowMainMenu();

    if (choice == 0)
    {
        InitLevel();
        InitPlayer();
        InitBombs();
        InitEnemies();

        while (!WindowShouldClose())
        {
            if (player.status)
            {
                UpdatePlayer();
            }
            UpdateEnemies();
            UpdateBombs();
            UpdateExplosions();

            BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawLevel();
            DrawBombs();
            DrawExplosions();
            DrawEnemies();

            // HUD centralizada acima do mapa
            int gridWidth = MAP_WIDTH * TILE_SIZE;
            int offsetX = (GetScreenWidth() - gridWidth) / 2;
            int offsetY = (GetScreenHeight() - (MAP_HEIGHT * TILE_SIZE)) / 2;

            // Mostrar bombas disponíveis
            char textoBombas[64];
            sprintf(textoBombas, "Bombas: %d / %d", player.bombasDisponiveis, player.bombasMaximas);
            DrawText(textoBombas, offsetX, offsetY - 50, 20, BLACK);

            // Mostrar alcance com efeito proporcional
            char textoAlcance[64] = "Alcance: ";
            int baseFontSize = 20 + (player.alcanceExplosao - 1) * 2; // aumenta fonte com power-ups
            char fogo[32] = "";

            for (int i = 0; i < player.alcanceExplosao; i++)
            {
                strcat(fogo, "🔥");
            }

            strcat(textoAlcance, fogo);

            int textWidth = MeasureText(textoAlcance, baseFontSize);
            DrawText(textoAlcance,
                     offsetX + gridWidth - textWidth,
                     offsetY - 50,
                     baseFontSize,
                     RED);

            if (player.status)
            {
                DrawPlayer();
                char pontosTexto[64];
                sprintf(pontosTexto, "Pontos: %d", player.pontuacao);
                DrawText(pontosTexto, 10, 10, 20, BLACK);
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
                    // Limpa teclas pendentes
                    for (int i = 0; i < 10; i++)
                        GetKeyPressed();
                    continue;
                }
            }

            EndDrawing();
        }
    }

    CloseWindow();
    return 0;
}
