#ifndef PAUSE_H
#define PAUSE_H

#include <stdbool.h>

typedef enum {
    PAUSE_RETURN = 0,
    PAUSE_SAVE,
    PAUSE_EXIT,
    PAUSE_RETURN_TO_MENU
} PauseAction;

// Exibe o menu de pause. 
// Se `isGameMode == true`: mostra opções de salvar, retornar, sair e voltar ao menu.
// Se `isGameMode == false`: remove a opção de salvar (ex: editor de mapas).
PauseAction ShowPauseMenu(bool isGameMode);

#endif
