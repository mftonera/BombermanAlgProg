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

int main(void)
{
    InitWindow(800, 800, "Mini Bomberman");
    SetTargetFPS(60);

    int choice = ShowMainMenu();

    if (choice == 0) // Novo jogo
    {
        InitLevel();
        InitPlayer();
        InitBombs();
        InitEnemies();
    }
    else if (choice == 1) // Continuar
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
        if (!CarregarMapaDeArquivo("mapas/mapa1.txt"))
        {
            printf("Erro ao carregar mapa personalizado.\n");
            InitLevel();
        }
        InitPlayer();
        InitBombs();
        InitEnemies();
    }
    else if (choice == 3)
    {
        AbrirEditorDeMapa();
        return 0;
    }
    else
    {
        CloseWindow();
        return 0;
    }

    // Limpa teclas pendentes após o menu
    for (int i = 0; i < 10; i++)
        GetKeyPressed();

    while (!WindowShouldClose())
    {
        if (player.status)
        {
            UpdatePlayer();
        }

        UpdateEnemies();
        UpdateBombs();
        UpdateExplosions();

        // Salvar com tecla P
        if (IsKeyPressed(KEY_P))
        {
            SalvarJogo();
        }

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
        int baseFontSize = 20 + (player.alcanceExplosao - 1) * 2;
        char fogo[32] = "";
        for (int i = 0; i < player.alcanceExplosao; i++)
            strcat(fogo, "🔥");
        strcat(textoAlcance, fogo);
        int textWidth = MeasureText(textoAlcance, baseFontSize);
        DrawText(textoAlcance, offsetX + gridWidth - textWidth, offsetY - 50, baseFontSize, RED);

        // Pontuação
        char pontosTexto[64];
        sprintf(pontosTexto, "Pontos: %d", player.pontuacao);
        DrawText(pontosTexto, 10, 10, 20, BLACK);

        if (player.status)
        {
            DrawPlayer();
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
                for (int i = 0; i < 10; i++)
                    GetKeyPressed();
                continue;
            }

            if (IsKeyPressed(KEY_ESCAPE))
            {
                break;
            }
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
