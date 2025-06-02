// src/player.h
#ifndef PLAYER_H
#define PLAYER_H

typedef struct {
    int x, y; // posição no grid
    int bombasDisponiveis;
    int bombasMaximas;
    int alcanceExplosao;
    int status; // 1 = vivo, 0 = morto
    int pontuacao;
} Player;

extern Player player;

void InitPlayer(void);
void UpdatePlayer(void);
void DrawPlayer(void);

#endif
