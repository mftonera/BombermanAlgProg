#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"
#include "menu.h"
#include "level.h"
#include "player.h"
#include "bomb.h"
#include "enemy.h"
#include "save.h"
#include "editor.h"
#include "pause.h"
#include "map_selector.h"

Color fundoPersonalizado = (Color){28, 20, 41, 255}; // cor personalizada de fundo

int main(void)
{
    InitWindow(800, 800, "Mini Bomberman"); // cria janela do jogo
    InitAudioDevice(); // Inicializa sistema de Som
    SetExitKey(KEY_NINE); // Setta tecla 9 para fechamento abrupto 
    SetTargetFPS(60); // setta fps em 60 

    // Carrega a fonte 
    Font customFont = LoadFont("resources/fonte.ttf");

    // carrega as texturas
    Texture2D playerTexture = LoadTexture("resources/player.png");
    Texture2D enemyTexture = LoadTexture("resources/enemy.png");
    Texture2D bombTexture = LoadTexture("resources/bomb.png");
    Texture2D groundTex = LoadTexture("resources/tiles.png");
    Texture2D wallPlain = LoadTexture("resources/wall_plain.png");
    Texture2D destructibleTex = LoadTexture("resources/destructible.png");
    Texture2D exitTex = LoadTexture("resources/exit.png");
    Texture2D powerupBombTex = LoadTexture("resources/powerup_bomb.png");
    Texture2D powerupRangeTex = LoadTexture("resources/powerup_range.png");
    Texture2D titleTexture = LoadTexture("resources/titulo.png");

    // carrega os sons
    Sound powerupSound = LoadSound("resources/powerup.wav");
    Sound boom = LoadSound("resources/explosion.wav");

    // loop de opções
    while (!WindowShouldClose())
    {
        while (GetKeyPressed() != 0) // limpa o buffer de entrada
            ;
        ;

        // exibe o main menu
        int choice = ShowMainMenu(customFont, titleTexture, fundoPersonalizado,
                                  playerTexture);

        if (choice == 4) // ultima opção fecha a janela
            break;

        if (choice == 0) // primeira opção, inicia novo jogo
        {
            InitLevel(); // carrega mapa
            InitPlayer(); // carrega player
            InitBombs(); // carrega bombas  
            InitEnemies(); // carrega inimigos
        }
        else if (choice == 1) // segunda opção (carregar o save)
        {
            if (!CarregarJogo())
            {
                InitLevel();
                InitPlayer();
                InitBombs();
                InitEnemies(); // carrega mapa, player, bombas e inimigos
            }
        }
        else if (choice == 2) // terceira opção, carrega mapa personalizado
        {
            char *path = SelecionarMapaPersonalizado(); // abre menu de seleção de mapas
            if (path != NULL) 
            {
                if (!CarregarMapaDeArquivo(path)) // carrega o mapa personalizado
                {
                    InitLevel();
                }
                free(path); // Libera memória
                InitPlayer();
                InitBombs();
                InitEnemies(); // carrega player, bombas e inimigos
            }
            else
            {
                continue; // volta ao menu principal caso seja cancelado
            }
        }
        else if (choice == 3) // quarta opção, carrega o editor
        {
            while (GetKeyPressed() != 0) // limpa entrada pressionada
                ;
            AbrirEditorDeMapa(); // abre editor
            continue; // retorna ao menu prinicpal
        }


        // loop do jogo
        while (!WindowShouldClose())
        {
            if (player.status) // se o jogador estiver vivo
            UpdatePlayer(powerupSound);
            UpdateBombs(boom);
            UpdateExplosions();
            UpdateEnemies(); // atualiza player, bombas, explosões e inimigos

            // caso seja pressionado ESC, entra no menu de pause 
            if (IsKeyPressed(KEY_ESCAPE))
            {
                PauseAction acao = ShowPauseMenu(choice != 2, customFont); // abre menu de pause de acordo com o modo | TRUE pra jogo, FALSE para mapa personalizado e editor
                if (acao == PAUSE_RETURN)
                {
                    continue; // volta ao jogo
                }
                else if (acao == PAUSE_RETURN_TO_MENU)
                {
                    break; // volta para o menu principal
                }
                else if (acao == PAUSE_EXIT) // sai do jogo/editor
                {
                    if (choice == 2)
                    {
                        break;
                    }
                    else
                    {
                        CloseWindow(); // fecha o jogo completamnete
                        return 0;
                    }
                }
                else if (acao == PAUSE_SAVE && choice != 2) // salva o jogo (não disponível em mapas personalizados)
                {
                    SalvarJogo();
                }
                else
                {
                }
            }

            // inicializa o desenho da tela
            BeginDrawing();
            ClearBackground(fundoPersonalizado);

            // desenha o mapa com os elementos 
            DrawLevel(
                groundTex,
                destructibleTex,
                wallPlain,
                exitTex,
                powerupBombTex,
                powerupRangeTex);

            DrawBombs(bombTexture); // desenha bomba com textura
            DrawExplosions(); // desenha explosões
            DrawEnemies(enemyTexture); // desenha inimigos com texutra


            // setters para desenho da HUD
            int gridWidth = MAP_WIDTH * TILE_SIZE;
            int offsetX = (GetScreenWidth() - gridWidth) / 2;
            int offsetY = (GetScreenHeight() - (MAP_HEIGHT * TILE_SIZE)) / 2;
            float hudFontSize = 30;
            float spacing = 1;

            // Exibe texto com as bombas
            char textoBombas[64];
            sprintf(textoBombas, "Bombas: %d / %d", player.bombasDisponiveis, player.bombasMaximas);
            DrawTextEx(customFont, textoBombas, (Vector2){offsetX, offsetY - 30}, hudFontSize, spacing, WHITE);

            // Exibe texto com o alcance
            char textoAlcance[64];
            sprintf(textoAlcance, "Alcance: %d", player.alcanceExplosao);
            DrawTextEx(customFont, textoAlcance, (Vector2){offsetX + gridWidth - 120, offsetY - 30}, hudFontSize, spacing, WHITE);

            // Exibe texto com os pontos
            char textoPontos[64];
            sprintf(textoPontos, "Pontos: %d", player.pontuacao);
            DrawTextEx(customFont, textoPontos, (Vector2){10, 10}, hudFontSize, spacing, WHITE);

            if (player.status)
            {
                DrawPlayer(playerTexture); // se o jogador estiver vivo, desenha ele na tela
            }
            else // caso morto :C, desenha tela de game over
            {
                DrawText("GAME  OVER", 250, 220, 40, RED);
                DrawText("Pressione R para reiniciar ou ESC para sair", 120, 280, 20, DARKGRAY);

                if (IsKeyPressed(KEY_R)) // caso jogador pressione R, reinicia o mapa (gera um novo)
                {
                    InitLevel();
                    InitPlayer();
                    InitBombs();
                    InitEnemies();
                    while (GetKeyPressed() != 0) // limpa buffer
                        ;
                    continue;
                }

                if (IsKeyPressed(KEY_ESCAPE))
                    break;
            }

            EndDrawing(); // finaliza desenho
        }
        choice = -1; // reseta escolha ao finalizar a jogatina
    }


    // descarrega texturas
    UnloadTexture(playerTexture);
    UnloadTexture(enemyTexture);
    UnloadTexture(bombTexture);
    UnloadTexture(groundTex);
    UnloadTexture(destructibleTex);
    UnloadTexture(wallPlain);
    UnloadTexture(exitTex);
    UnloadTexture(powerupBombTex);
    UnloadTexture(powerupRangeTex);
    UnloadTexture(titleTexture);

    // descarrega sons
    UnloadSound(powerupSound);
    UnloadSound(boom);

    // fecha sistema de som e janela
    CloseAudioDevice();
    CloseWindow();
    return 0; // finaliza programa
}