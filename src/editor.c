#include "raylib.h"
#include <stdio.h>
#include "editor.h"
#include "level.h"

static int cursorX = 0;
static int cursorY = 0;

void AbrirEditorDeMapa(void) {
    int tileSize = 32;
    for (int y = 0; y < MAP_HEIGHT; y++)
        for (int x = 0; x < MAP_WIDTH; x++)
            map[y][x] = TILE_VAZIO;

    while (!WindowShouldClose()) {
        // Movimento
        if (IsKeyPressed(KEY_RIGHT) && cursorX < MAP_WIDTH - 1) cursorX++;
        if (IsKeyPressed(KEY_LEFT)  && cursorX > 0) cursorX--;
        if (IsKeyPressed(KEY_DOWN)  && cursorY < MAP_HEIGHT - 1) cursorY++;
        if (IsKeyPressed(KEY_UP)    && cursorY > 0) cursorY--;

        // Edição
        if (IsKeyPressed(KEY_Q)) map[cursorY][cursorX] = TILE_PAREDE_INDESTRUTIVEL;
        if (IsKeyPressed(KEY_E)) map[cursorY][cursorX] = TILE_PAREDE_DESTRUTIVEL;
        if (IsKeyPressed(KEY_C)) map[cursorY][cursorX] = TILE_VAZIO;

        // Salvar
if (IsKeyPressed(KEY_ENTER)) {
    // Entrada de nome
    char nomeArquivo[64] = {0};
    int letras = 0;
    SetExitKey(0);  // desativa ESC padrão temporariamente

    bool digitandoNome = true;

    while (digitandoNome && !WindowShouldClose()) {
        int key = GetCharPressed();
        while (key > 0 && letras < 63) {
            if ((key >= 32) && (key <= 125)) {
                nomeArquivo[letras] = (char)key;
                letras++;
            }
            key = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE)) {
            // Cancela entrada de nome e volta pro editor
            digitandoNome = false;
            break;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Digite o nome do arquivo e pressione ENTER para salvar", 50, 100, 20, BLACK);
        DrawText("Backspace para cancelar", 50, 140, 20, GRAY);
        DrawText(nomeArquivo, 50, 180, 30, DARKBLUE);
        EndDrawing();

        if (IsKeyPressed(KEY_ENTER)) {
            if (letras > 0) {
                char caminhoFinal[128];
                snprintf(caminhoFinal, sizeof(caminhoFinal), "mapas/%s.txt", nomeArquivo);

                FILE *f = fopen(caminhoFinal, "w");
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
            digitandoNome = false; // volta pro editor
        }
    }

    SetExitKey(KEY_ESCAPE); // restaura o ESC padrão depois
}

        // Desenho
        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (int y = 0; y < MAP_HEIGHT; y++) {
            for (int x = 0; x < MAP_WIDTH; x++) {
                Rectangle tile = { x * tileSize, y * tileSize, tileSize, tileSize };
                Color cor = LIGHTGRAY;

                if (map[y][x] == TILE_PAREDE_INDESTRUTIVEL) cor = GRAY;
                else if (map[y][x] == TILE_PAREDE_DESTRUTIVEL) cor = BROWN;

                DrawRectangleRec(tile, cor);
                DrawRectangleLinesEx(tile, 1, DARKGRAY);
            }
        }

        // Cursor
        DrawRectangleLines(cursorX * tileSize, cursorY * tileSize, tileSize, tileSize, RED);
        DrawText("Q: Parede | E: Tijolo | C: Limpar | ESC: Salvar", 10, 10, 20, BLACK);

        EndDrawing();
    }
}
