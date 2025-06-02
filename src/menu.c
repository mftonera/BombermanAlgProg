// src/menu.c
#include "raylib.h"
#include "menu.h"
#include <stdio.h>
#include <stdbool.h>

#define MENU_OPTION_COUNT 4

static const char *menuOptions[MENU_OPTION_COUNT] = {
    "1. Jogar",
    "2. Continuar",
    "3. Carregar Mapa",
    "4. Sair"
};

bool SaveFileExists() {
    FILE *file = fopen("save/save.dat", "rb");
    if (file) {
        fclose(file);
        return true;
    }
    return false;
}

int ShowMainMenu(void) {
    int selected = 0;

    while (!WindowShouldClose()) {
        // Entrada do usuário
        if (IsKeyPressed(KEY_DOWN)) selected = (selected + 1) % MENU_OPTION_COUNT;
        if (IsKeyPressed(KEY_UP))   selected = (selected - 1 + MENU_OPTION_COUNT) % MENU_OPTION_COUNT;
        if (IsKeyPressed(KEY_ENTER)) return selected;

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Cálculos de centralização
        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();

        const char *title = "Mini Bomberman";
        int titleFontSize = 40;
        int optionFontSize = 20;
        int spacing = 40;

        int titleWidth = MeasureText(title, titleFontSize);
        int titleX = (screenWidth - titleWidth) / 2;
        int titleY = screenHeight / 5;

        DrawText(title, titleX, titleY, titleFontSize, BLACK);

        int menuStartY = titleY + 80;
        for (int i = 0; i < MENU_OPTION_COUNT; i++) {
            if (i == 1 && !SaveFileExists()) continue;  // oculta "Continuar" se não há save

            Color color = (i == selected) ? RED : DARKGRAY;

            const char *optionText = menuOptions[i];
            int textWidth = MeasureText(optionText, optionFontSize);
            int textX = (screenWidth - textWidth) / 2;
            int textY = menuStartY + i * spacing;

            DrawText(optionText, textX, textY, optionFontSize, color);
        }

        EndDrawing();
    }

    return 3; // Se fechar a janela, sai do jogo
}
