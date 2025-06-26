#ifndef PAUSE_H
#define PAUSE_H

#include <stdbool.h>

typedef enum {
    PAUSE_RETURN = 0,
    PAUSE_SAVE,
    PAUSE_EXIT,
    PAUSE_RETURN_TO_MENU
} PauseAction;

PauseAction ShowPauseMenu(bool isGameMode);

#endif
