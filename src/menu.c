#include "raylib.h"
#include <stdio.h>
#include "menu.h"

int ShowMainMenu(Font font, Texture2D titleTexture, Color background, Texture2D playerTexture){
    const char *opcoes[] = {
        "1. Novo Jogo",
        "2. Continuar",
        "3. Carregar Mapa",
        "4. Editor de Mapa",
        "5. Sair"};
    int total = 5; // Total de opções no menu
    int selecionado = 0; // Índice da opção selecionada

    // Carrega a fonte personalizada
    Font fonteMenu = LoadFont("resources/fonte.ttf");
    float fontSize = 30;

    // Aguarda o usuário soltar ENTER antes de começar
    while (IsKeyDown(KEY_ENTER))
    {
        BeginDrawing();
        ClearBackground(background);
        DrawText("Aguarde...", GetScreenWidth() / 2 - 80, GetScreenHeight() / 2, 20, (Color){28, 20, 41, 255});
        EndDrawing();
    }

    while (!WindowShouldClose())
    {
        // Logica de navegação do menu
        if (IsKeyPressed(KEY_DOWN))
            selecionado = (selecionado + 1) % total;
        if (IsKeyPressed(KEY_UP))
            selecionado = (selecionado - 1 + total) % total;

        // Ação ao pressionar ENTER
        if (IsKeyPressed(KEY_ENTER))
        {
            UnloadFont(fonteMenu); // libera fonte antes de sair
            return selecionado;
        }

        BeginDrawing();

        // Dimensões e posição base
        float playerScale = 8.5f;
        int spacing = 10;
        int margin = 20;

        int startX = GetScreenWidth() - (playerTexture.width * playerScale) - margin;
        int startY = GetScreenHeight() - (playerTexture.height * playerScale) - margin;

        // Desenhar imagens ampliadas no canto inferior direito
        DrawTextureEx(playerTexture, (Vector2){startX + 20, startY - 40}, 0.0f, playerScale, WHITE);
        ClearBackground(background);

        // Desenha a imagem do título no topo
        float escala = 0.4f; // ajuste conforme necessário (0.5 = metade do tamanho original)
        int titleWidth = titleTexture.width * escala;
        int titleX = (GetScreenWidth() - titleWidth) / 2;
        int titleY = 20;

        DrawTextureEx(titleTexture, (Vector2){titleX, titleY}, 0.0f, escala, WHITE);


        // Deixa a opção selecionada em destaque (DARKGRAY)
        for (int i = 0; i < total; i++)
        {
            Color cor = (i == selecionado) ? DARKGRAY : WHITE;
            Vector2 pos = {
                20,
                GetScreenHeight() - total * 45 - 40 + i * 45};
            DrawTextEx(fonteMenu, opcoes[i], pos, fontSize, spacing, cor);
        }

        EndDrawing();
    }

    UnloadFont(fonteMenu);
    return 4; // default: sair
}
