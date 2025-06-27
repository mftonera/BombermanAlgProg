#include "raylib.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "editor.h"
#include "level.h"

// posição da seleção no grid, começando em 0 0
static int cursorX = 0;
static int cursorY = 0;

void AbrirEditorDeMapa() {
    Font fonte = LoadFont("resources/fonte.ttf"); // carrega a fonte personalizada
    float fonteTamanho = 18; 
    float fonteEspacamento = 1; // setta o tamanho e o espaçamento da fonte

    //faz com que "limpe" o pressionamento do ENTER
    while (IsKeyDown(KEY_ENTER)) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Aguarde...", GetScreenWidth()/2 - 60, GetScreenHeight()/2, 20, GRAY);
        EndDrawing();
    }

    // setters da qtd de tiles, altura, largura do grid e o offset do grid (leva o mapa pro meio da tela) 
    int tileSize = 32;
    int gridWidth = MAP_WIDTH * tileSize;
    int gridHeight = MAP_HEIGHT * tileSize;
    int offsetX = (GetScreenWidth() - gridWidth) / 2;
    int offsetY = (GetScreenHeight() - gridHeight) / 2;

    // Inicializa o mapa com tiles vazios
    for (int y = 0; y < MAP_HEIGHT; y++)
        for (int x = 0; x < MAP_WIDTH; x++)
            map[y][x] = TILE_VAZIO;

    //setters booleanos para o modo de editar (enqt editando for true ele continua na tela, caso entre no modoSalvar = true, entra na tela de salvamento)        
    bool editando = true;
    bool modoSalvar = false;

    // Loop principal de edição
    while (editando && !WindowShouldClose()) {
        if (IsKeyPressed(KEY_RIGHT) && cursorX < MAP_WIDTH - 1) cursorX++;
        if (IsKeyPressed(KEY_LEFT)  && cursorX > 0) cursorX--;
        if (IsKeyPressed(KEY_DOWN)  && cursorY < MAP_HEIGHT - 1) cursorY++;
        if (IsKeyPressed(KEY_UP)    && cursorY > 0) cursorY--;

        // Teclas para posicionamento de Tiles (Paredes indestrutiveis e destrutiveis + Tile vazio)
        if (IsKeyPressed(KEY_Q)) map[cursorY][cursorX] = TILE_PAREDE_INDESTRUTIVEL;
        if (IsKeyPressed(KEY_E)) map[cursorY][cursorX] = TILE_PAREDE_DESTRUTIVEL;
        if (IsKeyPressed(KEY_C)) map[cursorY][cursorX] = TILE_VAZIO;

        // entra no modo de salvamento (com limpeza de tecla enter)
        if (IsKeyPressed(KEY_ENTER)) {
            while (IsKeyDown(KEY_ENTER)) {
                BeginDrawing();
                ClearBackground((Color){28, 20, 41, 255});
                DrawTextEx(fonte, "Aguarde...", (Vector2){10, 10}, fonteTamanho, fonteEspacamento, WHITE);
                EndDrawing();
            }
            modoSalvar = true; // de fato entra no modo de salvar   
        }

        // verifica o pressionamento do ESC, caso o usuário queira sair do modo de edição
        if (IsKeyPressed(KEY_ESCAPE)) {
            bool confirmarSaida = false; // saida inicialmente como false
            while (!WindowShouldClose()) {
                BeginDrawing(); // desenha uma janela para mostrar UI de indicações doq o usuário pode fazer
                ClearBackground((Color){28, 20, 41, 255});
                DrawTextEx(fonte, "Deseja sair do editor?", (Vector2){240, 180}, fonteTamanho, fonteEspacamento, WHITE);
                DrawTextEx(fonte, "ENTER: Sim    BACKSPACE: Cancelar", (Vector2){180, 240}, fonteTamanho, fonteEspacamento, WHITE);
                EndDrawing();

                if (IsKeyPressed(KEY_ENTER)) { confirmarSaida = true; break; } // pressionado enter, sai do modo de edição de mapa
                if (IsKeyPressed(KEY_BACKSPACE)) break; // ESC cancela essa função e volta pro modo de edição
            }
            if (confirmarSaida) break;
        }

        // Se no modo salvamento, Lê o nome do arquivo e salva
        if (modoSalvar) {
            char nomeArquivo[64] = {0}; // máximo de 64 caracteres
            int letras = 0; // para contagem de caracteres
            bool digitando = true; // booleano para que o estado seja digitrando

            while (digitando && !WindowShouldClose()) {
                int key = GetCharPressed(); // pega o caracter digitado
                while (key > 0 && letras < 63) {
                    if ((key >= 32) && (key <= 125))
                        nomeArquivo[letras++] = (char)key;
                    key = GetCharPressed(); // coloca esse caractér como nome do arquivo
                }

                // caso seja pressionado BACKSPACE, deleta a letra
                if (IsKeyPressed(KEY_BACKSPACE) && letras > 0)
                    nomeArquivo[--letras] = '\0';

                // caso seja apertado ENTER, salva o nome e as informações contidas nele (mapa personalizado) (W, B e vazio C)    
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
                    digitando = false; // digitando settado como falso
                    modoSalvar = false; // modoSalvar settado como falso -> volta pra edição de mapa
                }

                // UI para instruções do usuário
                BeginDrawing();
                ClearBackground((Color){28, 20, 41, 255});
                DrawTextEx(fonte, "Digite o nome do arquivo e pressione ENTER para salvar", (Vector2){50, 50}, fonteTamanho, fonteEspacamento, WHITE);
                DrawTextEx(fonte, "Backspace para corrigir", (Vector2){50, 90}, fonteTamanho, fonteEspacamento, WHITE);
                DrawTextEx(fonte, nomeArquivo, (Vector2){50, 130}, fonteTamanho, fonteEspacamento, RED); 
                EndDrawing();
            }
        }

        // começo da renderização do editor
        BeginDrawing();
        ClearBackground((Color){28, 20, 41, 255});


        for (int y = 0; y < MAP_HEIGHT; y++) {
            for (int x = 0; x < MAP_WIDTH; x++) { // percorre linha e coluna do mapa
                Rectangle tile = {
                    offsetX + x * tileSize, 
                    offsetY + y * tileSize, // offsets (mapa no meio)
                    tileSize,
                    tileSize // Altura e Largura do tile (32x32)
                };
                Color cor = LIGHTGRAY; // cor para tile vazio
                if (map[y][x] == TILE_PAREDE_INDESTRUTIVEL) cor = GRAY; // cor para tile parede INDESTRUTIVEL
                else if (map[y][x] == TILE_PAREDE_DESTRUTIVEL) cor = BROWN; // cor para tile parede DESTRUTIVEL
                DrawRectangleRec(tile, cor); // desenha os tiles
                DrawRectangleLinesEx(tile, 1, DARKGRAY); // desenha linhas de sepração
            }
        }

        // destaca o "cursor" em vermelho, para UX do usuário
        DrawRectangleLines(offsetX + cursorX * tileSize, offsetY + cursorY * tileSize, tileSize, tileSize, RED);

        // UI para indicando instruções para o usuário
        DrawTextEx(fonte, "Q: Parede | E: Tijolo | C: Limpar", (Vector2){10, 10}, fonteTamanho, fonteEspacamento, WHITE);
        DrawTextEx(fonte, "ENTER: Salvar | ESC: Menu", (Vector2){10, 50}, fonteTamanho, fonteEspacamento, WHITE);

        EndDrawing();
    }

    UnloadFont(fonte); // descarrega a fonte usada
    while (GetKeyPressed() != 0); // Limpador do buffer de entrada
}