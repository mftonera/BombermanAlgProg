#include "raylib.h"
#include "pause.h"

/*
Função que exibe o menu de pausa e retorna a ação escolhida pelo usuário
isGameMode indica se estamos no modo de jogo (true) ou editor & mapa personalizado (false)
*/
PauseAction ShowPauseMenu(bool isGameMode, Font font) {
    const char *optionsAll[] = {
        "1. Return",
        "2. Save Game",
        "3. Exit Game",
        "4. Return to Main Menu" // menu jogo normal
    };
    const char *optionsNoSave[] = {
        "1. Return",
        "2. Exit Editor",
        "3. Return to Main Menu" // menu editor & mapa personalizado
    };

    // Escolhe qual conjunto de opções usar com base no modo atual
    const char **options = isGameMode ? optionsAll : optionsNoSave;

    // IF-ELSE concatenado que define a quantidade de opções disponíveis com base no modo
    int optionCount = isGameMode ? 4 : 3;

    int selected = 0; // Índice da opção atualmente selecionada (inicia na primeira)

    // Loop principal do menu de pausa
    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_DOWN)) selected = (selected + 1) % optionCount; // avança para baixo
        if (IsKeyPressed(KEY_UP)) selected = (selected - 1 + optionCount) % optionCount; // avança para cima

        // inicia o desneho do menu de Pause
        BeginDrawing();
        ClearBackground((Color){28, 20, 41, 255});
        DrawTextEx(font, "Pausado", (Vector2){GetScreenWidth() / 2 - MeasureTextEx(font, "Pausado", 40, 1).x / 2, 100}, 40, 1, WHITE); // Desenha o título "Pausado" centralizado

        // Loop para desenhar todas as opções do menu
        for (int i = 0; i < optionCount; i++) {
            Color color = (i == selected) ? DARKGRAY : WHITE; // Destaca a opção selecionada
            DrawTextEx(font, options[i], (Vector2){GetScreenWidth() / 2 - MeasureTextEx(font, options[i], 20, 1).x / 2, 200 + i * 40}, 20, 1, color);
                // Desenha cada opção centralizada horizontalmente, com espaçamento vertical

        }

        EndDrawing();

        // Se o usuário pressionar ENTER, retorna a ação correspondente à opção selecionada
        if (IsKeyPressed(KEY_ENTER)) {
            if (isGameMode) {
                switch (selected) { 
                    case 0: return PAUSE_RETURN; // Voltar ao jogo
                    case 1: return PAUSE_SAVE; // Salvar
                    case 2: return PAUSE_EXIT; // Sair completamente
                    case 3: return PAUSE_RETURN_TO_MENU; // Voltar ao menu principal
                }
            } else {
                switch (selected) {
                    case 0: return PAUSE_RETURN; // Voltar ao editor/jogo
                    case 1: return PAUSE_EXIT; // Sair completamente
                    case 2: return PAUSE_RETURN_TO_MENU; // Voltar ao menu principal
                }
            }
        }

        if (IsKeyPressed(KEY_NINE)) return PAUSE_EXIT;  // tecla de emergência para sair da aplicação (por conta do ESC que é utilizado de outras formas no código)
    }

    return PAUSE_EXIT;
}