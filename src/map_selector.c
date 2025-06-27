    #include "raylib.h"       
    #include <stdio.h>          
    #include <stdlib.h>         
    #include <string.h>         
    #include <dirent.h>         
    #include "map_selector.h"   

    #define MAX_MAPAS 64        // Número máximo de arquivos de mapa que serão exibidos
    #define MAX_NOME 64         // Tamanho máximo do nome de cada arquivo

    // Função que permite ao usuário escolher um mapa personalizado dentro da pasta "mapas"
        char *SelecionarMapaPersonalizado(void) {
        DIR *dir = opendir("mapas");      // Abre o diretório "mapas"
        if (!dir) return NULL;            // Se não conseguir abrir, retorna NULL (volta pro menu)

        char nomes[MAX_MAPAS][MAX_NOME];  // Vetor para armazenar os nomes dos arquivos encontrados
        int total = 0;                    // Contador de arquivos válidos

        struct dirent *entry;
        // Loop para percorrer os arquivos no diretório
        while ((entry = readdir(dir)) != NULL && total < MAX_MAPAS) {
            // Filtra apenas arquivos com extensão .txt para serem mostrados (caso haja outros .bin / .dat perdidos não mostra)
            if (strstr(entry->d_name, ".txt")) {
                strncpy(nomes[total], entry->d_name, MAX_NOME - 1);  // Copia o nome do arquivo
                nomes[total][MAX_NOME - 1] = '\0';                   // Garante término nulo da string
                total++;                                             // Incrementa o contador
            }
        }
        closedir(dir);   // Fecha o diretório após leitura

        if (total == 0) return NULL;  // Se nenhum mapa for encontrado, retorna NULL (volta pro menu principal)

        int selecionado = 0;  // Índice do arquivo selecionado inicialmente

        // Solta o enter (shit happens otherwise :/)
        while (IsKeyDown(KEY_ENTER)) {
            BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("Aguarde...", 300, 400, 20, DARKGRAY); // nem aparece pq é muito rápido
            EndDrawing();
        }

        // Loop principal de seleção de mapa
        while (!WindowShouldClose()) {
            // Controle de navegação com as teclas de seta
            if (IsKeyPressed(KEY_DOWN)) selecionado = (selecionado + 1) % total; 
            if (IsKeyPressed(KEY_UP)) selecionado = (selecionado - 1 + total) % total;

            // Quando o usuário pressiona ENTER, abre a file selecionada
            if (IsKeyPressed(KEY_ENTER)) {
                char *caminho = malloc(128);  // Aloca memória para o caminho
                snprintf(caminho, 128, "mapas/%s", nomes[selecionado]);  // Formata o caminho
                return caminho;  // Retorna o caminho do arquivo selecionado
            }

            // Pressionar BACKSPACE no menu de seleção cancela a seleção (volta pro menu)
            if (IsKeyPressed(KEY_BACKSPACE)) return NULL;

            // Desenha a interface de seleção
            BeginDrawing();
            ClearBackground((Color){28, 20, 41, 255});  // Cor de fundo personalizada
            DrawText("Selecione um mapa personalizado", 80, 50, 30, WHITE); // "Desenha" texto de UI

            // Lista os nomes dos arquivos encontrados
            for (int i = 0; i < total; i++) {
                Color cor = (i == selecionado) ? RED : WHITE;  // Destaca o item selecionado (vermelho) o resto fica branco
                DrawText(nomes[i], 100, 100 + i * 30, 20, cor);  // "Desenha" o nome dos mapas
            }

            DrawText("ENTER: Carregar  |  BACKSPACE: Cancelar", 80, 700, 20, WHITE); // "Desenha as instruções de navegação (UI)"
            EndDrawing();
        }

        return NULL;  // Se a janela for fechada, retorna NULL (volta pro menu principal)
    }