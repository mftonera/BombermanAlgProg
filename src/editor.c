#include "raylib.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "editor.h"
#include "level.h"

static int cursorX = 0;
static int cursorY = 0;

void AbrirEditorDeMapa() {
    Font fonte = LoadFont("resources/fonte.ttf");
    float fonteTamanho = 18;
    float fonteEspacamento = 1;

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

    for (int y = 0; y < MAP_HEIGHT; y++)
        for (int x = 0; x < MAP_WIDTH; x++)
            map[y][x] = TILE_VAZIO;

    bool editando = true;
    bool modoSalvar = false;

    while (editando && !WindowShouldClose()) {
        if (IsKeyPressed(KEY_RIGHT) && cursorX < MAP_WIDTH - 1) cursorX++;
        if (IsKeyPressed(KEY_LEFT)  && cursorX > 0) cursorX--;
        if (IsKeyPressed(KEY_DOWN)  && cursorY < MAP_HEIGHT - 1) cursorY++;
        if (IsKeyPressed(KEY_UP)    && cursorY > 0) cursorY--;

        if (IsKeyPressed(KEY_Q)) map[cursorY][cursorX] = TILE_PAREDE_INDESTRUTIVEL;
        if (IsKeyPressed(KEY_E)) map[cursorY][cursorX] = TILE_PAREDE_DESTRUTIVEL;
        if (IsKeyPressed(KEY_C)) map[cursorY][cursorX] = TILE_VAZIO;

        if (IsKeyPressed(KEY_ENTER)) {
            while (IsKeyDown(KEY_ENTER)) {
                BeginDrawing();
                ClearBackground(RAYWHITE);
                DrawTextEx(fonte, "Aguarde...", (Vector2){10, 10}, fonteTamanho, fonteEspacamento, GRAY);
                EndDrawing();
            }
            modoSalvar = true;
        }

        if (IsKeyPressed(KEY_ESCAPE)) {
            bool confirmarSaida = false;
            while (!WindowShouldClose()) {
                BeginDrawing();
                ClearBackground(RAYWHITE);
                DrawTextEx(fonte, "Deseja sair do editor?", (Vector2){240, 180}, fonteTamanho, fonteEspacamento, BLACK);
                DrawTextEx(fonte, "ENTER: Sim    BACKSPACE: Cancelar", (Vector2){180, 240}, fonteTamanho, fonteEspacamento, DARKGRAY);
                EndDrawing();

                if (IsKeyPressed(KEY_ENTER)) { confirmarSaida = true; break; }
                if (IsKeyPressed(KEY_BACKSPACE)) break;
            }
            if (confirmarSaida) break;
        }

        if (modoSalvar) {
            char nomeArquivo[64] = {0};
            int letras = 0;
            bool digitando = true;

            while (digitando && !WindowShouldClose()) {
                int key = GetCharPressed();
                while (key > 0 && letras < 63) {
                    if ((key >= 32) && (key <= 125))
                        nomeArquivo[letras++] = (char)key;
                    key = GetCharPressed();
                }

                if (IsKeyPressed(KEY_BACKSPACE) && letras > 0)
                    nomeArquivo[--letras] = '\0';

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
                    modoSalvar = false;
                }

                BeginDrawing();
                ClearBackground(RAYWHITE);
                DrawTextEx(fonte, "Digite o nome do arquivo e pressione ENTER para salvar", (Vector2){50, 50}, fonteTamanho, fonteEspacamento, BLACK);
                DrawTextEx(fonte, "Backspace para corrigir", (Vector2){50, 90}, fonteTamanho, fonteEspacamento, GRAY);
                DrawTextEx(fonte, nomeArquivo, (Vector2){50, 130}, fonteTamanho, fonteEspacamento, DARKBLUE);
                EndDrawing();
            }
        }

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

        DrawRectangleLines(offsetX + cursorX * tileSize, offsetY + cursorY * tileSize, tileSize, tileSize, RED);

        DrawTextEx(fonte, "Q: Parede | E: Tijolo | C: Limpar", (Vector2){10, 10}, fonteTamanho, fonteEspacamento, BLACK);
        DrawTextEx(fonte, "ENTER: Salvar | ESC: Menu", (Vector2){10, 50}, fonteTamanho, fonteEspacamento, DARKGRAY);

        EndDrawing();
    }

    UnloadFont(fonte);
    while (GetKeyPressed() != 0);
}
