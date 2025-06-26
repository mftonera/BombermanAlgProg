#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "map_selector.h"

#define MAX_MAPAS 64
#define MAX_NOME 64

char *SelecionarMapaPersonalizado(void) {
    DIR *dir = opendir("mapas");
    if (!dir) return NULL;

    char nomes[MAX_MAPAS][MAX_NOME];
    int total = 0;

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL && total < MAX_MAPAS) {
        if (strstr(entry->d_name, ".txt")) {
            strncpy(nomes[total], entry->d_name, MAX_NOME - 1);
            nomes[total][MAX_NOME - 1] = '\0';
            total++;
        }
    }
    closedir(dir);

    if (total == 0) return NULL;

    int selecionado = 0;

    while (IsKeyDown(KEY_ENTER)) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Aguarde...", 300, 400, 20, DARKGRAY);
        EndDrawing();
    }

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_DOWN)) selecionado = (selecionado + 1) % total;
        if (IsKeyPressed(KEY_UP)) selecionado = (selecionado - 1 + total) % total;

        if (IsKeyPressed(KEY_ENTER)) {
            char *caminho = malloc(128);
            snprintf(caminho, 128, "mapas/%s", nomes[selecionado]);
            return caminho;
        }

        if (IsKeyPressed(KEY_BACKSPACE)) return NULL;

        BeginDrawing();
        ClearBackground((Color){28, 20, 41, 255});
        DrawText("Selecione um mapa personalizado", 80, 50, 30, WHITE);

        for (int i = 0; i < total; i++) {
            Color cor = (i == selecionado) ? RED : WHITE;
            DrawText(nomes[i], 100, 100 + i * 30, 20, cor);
        }

        DrawText("ENTER: Carregar  |  BACKSPACE: Cancelar", 80, 700, 20, WHITE);
        EndDrawing();
    }

    return NULL;
}
