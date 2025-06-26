#include "raylib.h"
#include <stdio.h>
#include "menu.h"

int ShowMainMenu(void) {
    const char *opcoes[] = {
        "1. Novo Jogo",
        "2. Continuar",
        "3. Carregar Mapa",
        "4. Editor de Mapa",
        "5. Sair"
    };
    int total = 5;
    int selecionado = 0;

    // Carrega a fonte personalizada
    Font fonteMenu = LoadFont("resources/fonte.ttf");
    float fontSize = 30;
    float spacing = 1;

    // ✅ Aguarda o usuário soltar ENTER antes de começar
    while (IsKeyDown(KEY_ENTER)) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Aguarde...", GetScreenWidth() / 2 - 80, GetScreenHeight() / 2, 20, DARKGRAY);
        EndDrawing();
    }

    while (!WindowShouldClose()) {
        // Movimento
        if (IsKeyPressed(KEY_DOWN)) selecionado = (selecionado + 1) % total;
        if (IsKeyPressed(KEY_UP)) selecionado = (selecionado - 1 + total) % total;

        // Escolha
        if (IsKeyPressed(KEY_ENTER)) {
            UnloadFont(fonteMenu); // libera fonte antes de sair
            return selecionado;
        }

        // Desenho
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("Mini Bomberman", GetScreenWidth() / 2 - MeasureText("Mini Bomberman", 40) / 2, 100, 40, BLACK);

        for (int i = 0; i < total; i++) {
            Color cor = (i == selecionado) ? RED : DARKGRAY;
            Vector2 pos = {
                20,
                GetScreenHeight() - total * 45 - 40 + i * 45
            };
            DrawTextEx(fonteMenu, opcoes[i], pos, fontSize, spacing, cor);
        }

        EndDrawing();
    }

    UnloadFont(fonteMenu);
    return 4; // default: sair
}
