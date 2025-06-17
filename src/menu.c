// src/menu.c
#include "raylib.h"
#include "menu.h"
#include "save.h"
#include <stdio.h>
#include <stdbool.h>

#define MENU_OPTION_COUNT 4

static const char *menuOptions[MENU_OPTION_COUNT] = {
    "1. Jogar",
    "2. Continuar",
    "3. Carregar Mapa",
    "4. Editor de Mapa",
    "5. Sair",
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
    const char *menuOptions[] = {
        "1. Novo Jogo",
        "2. Continuar",
        "3. Carregar Mapa",
        "4. Editor de Mapa",
        "5. Sair",
    };

    int selected = 0;
    int totalOptions = 4;

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_DOWN)) {
            do {
                selected = (selected + 1) % totalOptions;
            } while (selected == 1 && !SaveFileExiste()); // pula "Continuar" se não houver save
        }

        if (IsKeyPressed(KEY_UP)) {
            do {
                selected = (selected - 1 + totalOptions) % totalOptions;
            } while (selected == 1 && !SaveFileExiste()); // pula "Continuar" se não houver save
        }

        if (IsKeyPressed(KEY_ENTER)) {
            // Se selecionou "Continuar" e não existe save, ignora
            if (selected == 1 && !SaveFileExiste()) continue;
            return selected;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("Mini Bomberman", GetScreenWidth()/2 - MeasureText("Mini Bomberman", 40)/2, 100, 40, BLACK);

        int visibleIndex = 0;
        for (int i = 0; i < totalOptions; i++) {
            if (i == 1 && !SaveFileExiste()) continue; // oculta “Continuar” se não houver save

            Color color = (i == selected) ? RED : DARKGRAY;
            int textWidth = MeasureText(menuOptions[i], 20);
            DrawText(menuOptions[i],
                     GetScreenWidth() / 2 - textWidth / 2,
                     200 + visibleIndex * 40,
                     20, color);
            visibleIndex++;
        }

        EndDrawing();
    }

    return 3; // ESC ou fechar janela
}

