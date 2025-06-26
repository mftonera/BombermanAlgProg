#include "raylib.h"
#include "pause.h"

PauseAction ShowPauseMenu(bool isGameMode, Font font) {
    const char *optionsAll[] = {
        "1. Return",
        "2. Save Game",
        "3. Exit Game",
        "4. Return to Main Menu"
    };
    const char *optionsNoSave[] = {
        "1. Return",
        "2. Exit Editor",
        "3. Return to Main Menu"
    };

    const char **options = isGameMode ? optionsAll : optionsNoSave;
    int optionCount = isGameMode ? 4 : 3;

    int selected = 0;

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_DOWN)) selected = (selected + 1) % optionCount;
        if (IsKeyPressed(KEY_UP)) selected = (selected - 1 + optionCount) % optionCount;

        BeginDrawing();
        ClearBackground((Color){28, 20, 41, 255});
        DrawTextEx(font, "Pausado", (Vector2){GetScreenWidth() / 2 - MeasureTextEx(font, "Pausado", 40, 1).x / 2, 100}, 40, 1, WHITE);

        for (int i = 0; i < optionCount; i++) {
            Color color = (i == selected) ? DARKGRAY : WHITE;
            DrawTextEx(font, options[i], (Vector2){GetScreenWidth() / 2 - MeasureTextEx(font, options[i], 20, 1).x / 2, 200 + i * 40}, 20, 1, color);

        }

        EndDrawing();

        if (IsKeyPressed(KEY_ENTER)) {
            if (isGameMode) {
                switch (selected) { 
                    case 0: return PAUSE_RETURN;
                    case 1: return PAUSE_SAVE;
                    case 2: return PAUSE_EXIT;
                    case 3: return PAUSE_RETURN_TO_MENU;
                }
            } else {
                switch (selected) {
                    case 0: return PAUSE_RETURN;
                    case 1: return PAUSE_EXIT;
                    case 2: return PAUSE_RETURN_TO_MENU;
                }
            }
        }

        if (IsKeyPressed(KEY_NINE)) return PAUSE_EXIT;  // tecla extra para sair de emergência
    }

    return PAUSE_EXIT;
}
