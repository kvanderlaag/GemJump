#include <curses.h>

typedef struct level {
    char* name;
    unsigned char rows;
    unsigned char cols;
    unsigned char time;
    int playerStartRow;
    int playerStartCol;
    int gems;
    struct entity* entityList;
    char* viewData;

} level;

struct entity {
    unsigned char type;
    unsigned char length;
    unsigned char row;
    unsigned char col;
    struct entity* next;
};

level load_level(char* filename);

void print_level_data(level* activeLevel, WINDOW* win);

void display_level(level* activeLevel, int row, int col, WINDOW* win);

void initialize_level(level* activeLevel);
