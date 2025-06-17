#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "save.h"
#include "player.h"
#include "level.h"
#include "bomb.h"
#include "enemy.h"

#define SAVE_PATH "save/save.dat"

typedef struct {
    Player player;
    int map[MAP_HEIGHT][MAP_WIDTH];
    Bomb bombs[MAX_BOMBS];
    Explosion explosions[MAX_EXPLOSIONS];
    Enemy enemies[MAX_ENEMIES];
} SaveData;

void SalvarJogo(void) {
    FILE *f = fopen(SAVE_PATH, "wb");
    if (!f) return;

    SaveData data;
    data.player = player;
    memcpy(data.map, map, sizeof(map));
    memcpy(data.bombs, bombs, sizeof(bombs));
    memcpy(data.explosions, explosions, sizeof(explosions));
    memcpy(data.enemies, enemies, sizeof(enemies));

    fwrite(&data, sizeof(SaveData), 1, f);
    fclose(f);
}

bool CarregarJogo(void) {
    FILE *f = fopen(SAVE_PATH, "rb");
    if (!f) return false;

    SaveData data;
    fread(&data, sizeof(SaveData), 1, f);
    fclose(f);

    player = data.player;
    memcpy(map, data.map, sizeof(map));
    memcpy(bombs, data.bombs, sizeof(bombs));
    memcpy(explosions, data.explosions, sizeof(explosions));
    memcpy(enemies, data.enemies, sizeof(enemies));

    return true;
}

bool SaveFileExiste(void) {
    FILE *f = fopen(SAVE_PATH, "rb");
    if (f) {
        fclose(f);
        return true;
    }
    return false;
}
