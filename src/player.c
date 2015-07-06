#include <levels.h>
#include <player.h>
#include <values.h>

player init_player(level* activeLevel) {
    player newPlayer;
    newPlayer.row = activeLevel->playerStartRow;
    newPlayer.col = activeLevel->playerStartCol;
    newPlayer.jumping = 0;
    return newPlayer;
}

void move_player(player* currentPlayer, level* activeLevel, char dir) {
    unsigned char* left = activeLevel->viewData + (currentPlayer->row * activeLevel->cols) + currentPlayer->col - 1;
    unsigned char* right = activeLevel->viewData + (currentPlayer->row * activeLevel->cols) + currentPlayer->col + 1;
    switch (dir) {
        case 'l':
            if ((currentPlayer->col > 0) && ((*left) != PLATFORM_CH)) {
                currentPlayer->col--;
            }
            break;
        case 'r':
            if ((currentPlayer->col < activeLevel->cols - 1) && ((*right) != PLATFORM_CH))  {
                currentPlayer->col++;
            }
            break;
        case 'u':
            break;

    }
    return;
}

void display_player(player* currentPlayer, level* activeLevel, WINDOW* win, int rowOffset, int colOffset) {
    wmove(win, currentPlayer->row - rowOffset + 1, currentPlayer->col - colOffset + 1);
    waddch(win, PLAYER_CH);
    return;
}

void check_gravity(player* currentPlayer, level* activeLevel) {
    if (currentPlayer->jumping == 0) {
        if (currentPlayer->row < (activeLevel->rows -1)) {
            unsigned char* below = activeLevel->viewData + ((currentPlayer->row + 1) * activeLevel->cols) + currentPlayer->col;
            switch (*below ) {
                case PLATFORM_CH:
                    break;
                default:
                    currentPlayer->row++;
            }
        }
    } else {
        unsigned char* above = activeLevel->viewData + ((currentPlayer->row - 1) * activeLevel->cols) + currentPlayer->col;
        if ( (*above != PLATFORM_CH) && (currentPlayer->row > 0) ){
            currentPlayer->row--;
        }
        currentPlayer->jumping--;
    }
}

void jump(player* currentPlayer, level* activeLevel) {
    if ( (currentPlayer->row > 0) && (currentPlayer->jumping == 0) ) {
        unsigned char* below = activeLevel->viewData + ((currentPlayer->row + 1) * activeLevel->cols) + currentPlayer->col;
        if ( (*below == PLATFORM_CH) || (currentPlayer->row == activeLevel->rows - 1) ){
            currentPlayer->jumping = JUMP_MAX;
        }
    }
}

void check_gems(player* currentPlayer, level* activeLevel) {
    char* found = activeLevel->viewData + (currentPlayer->row * activeLevel->cols) + currentPlayer->col;
    if (*found == GEM_CH) {
        activeLevel->gems--;
        *(activeLevel->viewData + (currentPlayer->row * activeLevel->cols) + currentPlayer->col) = ' ';
    }
}
