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

    Texture2D playerTexture = LoadTexture("resources/player.png");
    Texture2D enemyTexture = LoadTexture("resources/enemy.png");
    Texture2D bombTexture = LoadTexture("resources/bomb.png");

    Texture2D groundTex = LoadTexture("resources/ground.png");
    Texture2D destructibleWallTex = LoadTexture("resources/destructible_wall.png");
    Texture2D wallTopTex = LoadTexture("resources/wall_top.png");
    Texture2D wallBottomTex = LoadTexture("resources/wall_bottom.png");
    Texture2D wallSideLeftTex = LoadTexture("resources/wall_side_left.png");
    Texture2D wallSideRightTex = LoadTexture("resources/wall_side_right.png");
    Texture2D wallCornerTL_Tex = LoadTexture("resources/wall_corner_tl.png");
    Texture2D wallCornerTR_Tex = LoadTexture("resources/wall_corner_tr.png");
    Texture2D wallCornerBL_Tex = LoadTexture("resources/wall_corner_bl.png");
    Texture2D wallCornerBR_Tex = LoadTexture("resources/wall_corner_br.png");
    Texture2D wallMiddleTex = LoadTexture("resources/wall_middle.png");

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

            DrawLevel(groundTex, 
                destructibleWallTex, 
                wallTopTex, 
                wallBottomTex, 
                wallSideLeftTex,
                wallSideRightTex,
                wallCornerTL_Tex, 
                wallCornerTR_Tex, 
                wallCornerBL_Tex, 
                wallCornerBR_Tex, 
                wallMiddleTex
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
            for (int i = 0; i < player.alcanceExplosao; i++) strcat(fogo, "🔥");
            strcat(textoAlcance, fogo);
            int textWidth = MeasureText(textoAlcance, baseFontSize);
            DrawText(textoAlcance, offsetX + gridWidth - textWidth, offsetY - 50, baseFontSize, RED);

            char pontosTexto[64];
            sprintf(pontosTexto, "Pontos: %d", player.pontuacao);
            DrawText(pontosTexto, 10, 10, 20, BLACK);

            if (player.status) {
                DrawPlayer(playerTexture);
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

    UnloadTexture(playerTexture);
    UnloadTexture(enemyTexture);
    UnloadTexture(bombTexture);
    UnloadTexture(groundTex);
    UnloadTexture(destructibleWallTex);
    UnloadTexture(wallTopTex);
    UnloadTexture(wallBottomTex);
    UnloadTexture(wallSideLeftTex);
    UnloadTexture(wallSideRightTex);
    UnloadTexture(wallCornerTL_Tex);
    UnloadTexture(wallCornerTR_Tex);
    UnloadTexture(wallCornerBL_Tex);
    UnloadTexture(wallCornerBR_Tex);
    UnloadTexture(wallMiddleTex);

    CloseWindow();
    return 0;
}
