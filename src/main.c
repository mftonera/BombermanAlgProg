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

int main(void)
{
    InitWindow(800, 800, "Mini Bomberman");
    SetExitKey(KEY_NINE);
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        while (GetKeyPressed() != 0);  // limpa buffer de teclas

        int choice = ShowMainMenu();
        if (choice == 4) break; // Sair

        if (choice == 0) { // Novo jogo
            InitLevel();
            InitPlayer();
            InitBombs();
            InitEnemies();
        }
        else if (choice == 1) { // Continuar
            if (!CarregarJogo()) {
                InitLevel();
                InitPlayer();
                InitBombs();
                InitEnemies();
            }
        }
        else if (choice == 2) { // Carregar mapa personalizado
            char *path = SelecionarMapaPersonalizado();
            if (path != NULL) {
                if (!CarregarMapaDeArquivo(path)) {
                    InitLevel();  // fallback se erro no arquivo
                }
                free(path);
                InitPlayer();
                InitBombs();
                InitEnemies();
            } else {
                continue; // usuário cancelou, volta ao menu
            }
        }
        else if (choice == 3) { // Editor
            while (GetKeyPressed() != 0);
            AbrirEditorDeMapa();
            continue;
        }

        // Loop principal do jogo
        while (!WindowShouldClose())
        {
            if (player.status) UpdatePlayer();
            UpdateBombs();
            UpdateExplosions();
            UpdateEnemies();

            if (IsKeyPressed(KEY_ESCAPE)) {
                int podeSalvar = (choice != 2); // não salva se for mapa personalizado
                PauseAction acao = ShowPauseMenu(podeSalvar);

                if (acao == PAUSE_RETURN_TO_MENU) break;
                if (acao == PAUSE_EXIT) {
                    CloseWindow();
                    return 0;
                }
                if (acao == PAUSE_SAVE && podeSalvar) {
                    SalvarJogo();
                }
            }

            BeginDrawing();
            ClearBackground(RAYWHITE);

            DrawLevel();
            DrawBombs();
            DrawExplosions();
            DrawEnemies();

            int gridWidth = MAP_WIDTH * TILE_SIZE;
            int offsetX = (GetScreenWidth() - gridWidth) / 2;
            int offsetY = (GetScreenHeight() - (MAP_HEIGHT * TILE_SIZE)) / 2;

            char textoBombas[64];
            sprintf(textoBombas, "Bombas: %d / %d", player.bombasDisponiveis, player.bombasMaximas);
            DrawText(textoBombas, offsetX, offsetY - 50, 20, BLACK);

            char textoAlcance[64] = "Alcance: ";
            int baseFontSize = 20 + (player.alcanceExplosao - 1) * 2;
            char fogo[32] = "";
            for (int i = 0; i < player.alcanceExplosao; i++) strcat(fogo, "🔥");
            strcat(textoAlcance, fogo);
            int textWidth = MeasureText(textoAlcance, baseFontSize);
            DrawText(textoAlcance, offsetX + gridWidth - textWidth, offsetY - 50, baseFontSize, RED);

            char pontosTexto[64];
            sprintf(pontosTexto, "Pontos: %d", player.pontuacao);
            DrawText(pontosTexto, 10, 10, 20, BLACK);

            if (player.status) {
                DrawPlayer();
            }
            else {
                DrawText("GAME  OVER", 250, 220, 40, RED);
                DrawText("Pressione R para reiniciar ou ESC para sair", 120, 280, 20, DARKGRAY);

                if (IsKeyPressed(KEY_R)) {
                    InitLevel();
                    InitPlayer();
                    InitBombs();
                    InitEnemies();
                    while (GetKeyPressed() != 0);
                    continue;
                }

                if (IsKeyPressed(KEY_ESCAPE)) break;
            }

            EndDrawing();
        }
    }
    
    UnloadEnemies();
    CloseWindow();
    return 0;
}
