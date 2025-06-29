// src/player.h
#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

typedef struct {
    int x, y; // posição no grid
    int bombasDisponiveis;
    int bombasMaximas;
    int alcanceExplosao;
    int status; // 1 = vivo, 0 = morto
    int pontuacao;
    int direcaoSprite; // -1 = direita, 1 = esquerda
} Player;

extern Player player;

void InitPlayer(void);
void UpdatePlayer(Sound powerupSound);
void DrawPlayer(Texture2D playerTexture);

#endif
