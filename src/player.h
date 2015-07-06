typedef struct player {
    unsigned char row;
    unsigned char col;
    int jumping;
} player;

player init_player(level* activeLevel);

void move_player(player* currentPlayer, level* activeLevel, char dir);

void display_player(player* currentPlayer, level* activeLevel, WINDOW* win, int rowOffset, int colOffset);

void check_gravity(player* currentPlayer, level* activeLevel);

void jump(player* currentPlayer, level* activeLevel);

void check_gems(player* currentPlayer, level* activeLevel);
