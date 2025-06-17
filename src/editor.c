#include "raylib.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "editor.h"
#include "level.h"

static int cursorX = 0;
static int cursorY = 0;

void AbrirEditorDeMapa(void) {
    // ✅ Aguarda ENTER ser solto antes de iniciar
    while (IsKeyDown(KEY_ENTER)) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Aguarde...", GetScreenWidth()/2 - 60, GetScreenHeight()/2, 20, GRAY);
        EndDrawing();
    }

    int tileSize = 32;
    int gridWidth = MAP_WIDTH * tileSize;
    int gridHeight = MAP_HEIGHT * tileSize;
    int offsetX = (GetScreenWidth() - gridWidth) / 2;
    int offsetY = (GetScreenHeight() - gridHeight) / 2;

    // Limpa o mapa
    for (int y = 0; y < MAP_HEIGHT; y++)
        for (int x = 0; x < MAP_WIDTH; x++)
            map[y][x] = TILE_VAZIO;

    bool editando = true;
    bool modoSalvar = false;

    while (editando && !WindowShouldClose()) {
        // Movimento
        if (IsKeyPressed(KEY_RIGHT) && cursorX < MAP_WIDTH - 1) cursorX++;
        if (IsKeyPressed(KEY_LEFT)  && cursorX > 0) cursorX--;
        if (IsKeyPressed(KEY_DOWN)  && cursorY < MAP_HEIGHT - 1) cursorY++;
        if (IsKeyPressed(KEY_UP)    && cursorY > 0) cursorY--;

        // Edição
        if (IsKeyPressed(KEY_Q)) map[cursorY][cursorX] = TILE_PAREDE_INDESTRUTIVEL;
        if (IsKeyPressed(KEY_E)) map[cursorY][cursorX] = TILE_PAREDE_DESTRUTIVEL;
        if (IsKeyPressed(KEY_C)) map[cursorY][cursorX] = TILE_VAZIO;

        // Salvar ao pressionar ENTER
        if (IsKeyPressed(KEY_ENTER)) {
            // Aguarda soltar ENTER para evitar flicker
            while (IsKeyDown(KEY_ENTER)) {
                BeginDrawing();
                ClearBackground(RAYWHITE);
                DrawText("Aguarde...", 10, 10, 20, GRAY);
                EndDrawing();
            }
            modoSalvar = true;
        }

        // ESC para sair com confirmação
        if (IsKeyPressed(KEY_ESCAPE)) {
            bool confirmarSaida = false;
            while (!WindowShouldClose()) {
                BeginDrawing();
                ClearBackground(RAYWHITE);
                DrawText("Deseja sair do editor?", 240, 280, 20, BLACK);
                DrawText("ENTER: Sim    BACKSPACE: Cancelar", 180, 320, 20, DARKGRAY);
                EndDrawing();

                if (IsKeyPressed(KEY_ENTER)) {
                    confirmarSaida = true;
                    break;
                }
                if (IsKeyPressed(KEY_BACKSPACE)) break;
            }
            if (confirmarSaida) break;
        }

        // =================== SALVAR MAPA ===================
        if (modoSalvar) {
            char nomeArquivo[64] = {0};
            int letras = 0;
            bool digitando = true;

            while (digitando && !WindowShouldClose()) {
                int key = GetCharPressed();
                while (key > 0 && letras < 63) {
                    if ((key >= 32) && (key <= 125)) {
                        nomeArquivo[letras++] = (char)key;
                    }
                    key = GetCharPressed();
                }

                if (IsKeyPressed(KEY_BACKSPACE) && letras > 0) {
                    letras--;
                    nomeArquivo[letras] = '\0';
                }

                if (IsKeyPressed(KEY_ENTER)) {
                    if (letras > 0) {
                        char caminho[128];
                        snprintf(caminho, sizeof(caminho), "mapas/%s.txt", nomeArquivo);
                        FILE *f = fopen(caminho, "w");
                        if (f) {
                            for (int y = 0; y < MAP_HEIGHT; y++) {
                                for (int x = 0; x < MAP_WIDTH; x++) {
                                    char c = ' ';
                                    if (map[y][x] == TILE_PAREDE_INDESTRUTIVEL) c = 'W';
                                    else if (map[y][x] == TILE_PAREDE_DESTRUTIVEL) c = 'B';
                                    fputc(c, f);
                                }
                                fputc('\n', f);
                            }
                            fclose(f);
                        }
                    }
                    digitando = false;
                    modoSalvar = false; // volta ao editor
                }

                BeginDrawing();
                ClearBackground(RAYWHITE);
                DrawText("Digite o nome do arquivo e pressione ENTER para salvar", 50, 100, 20, BLACK);
                DrawText("Backspace para corrigir", 50, 140, 20, GRAY);
                DrawText(nomeArquivo, 50, 180, 30, DARKBLUE);
                EndDrawing();
            }
        }

        // =================== DESENHO ===================
        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (int y = 0; y < MAP_HEIGHT; y++) {
            for (int x = 0; x < MAP_WIDTH; x++) {
                Rectangle tile = {
                    offsetX + x * tileSize,
                    offsetY + y * tileSize,
                    tileSize,
                    tileSize
                };
                Color cor = LIGHTGRAY;
                if (map[y][x] == TILE_PAREDE_INDESTRUTIVEL) cor = GRAY;
                else if (map[y][x] == TILE_PAREDE_DESTRUTIVEL) cor = BROWN;
                DrawRectangleRec(tile, cor);
                DrawRectangleLinesEx(tile, 1, DARKGRAY);
            }
        }

        DrawRectangleLines(
            offsetX + cursorX * tileSize,
            offsetY + cursorY * tileSize,
            tileSize, tileSize, RED);

        DrawText("Q: Parede | E: Tijolo | C: Limpar", 10, 10, 20, BLACK);
        DrawText("ENTER: Salvar | ESC: Menu", 10, 35, 18, DARKGRAY);

        EndDrawing();
    }

    // Limpa buffer de teclas ao sair
    while (GetKeyPressed() != 0);
}
