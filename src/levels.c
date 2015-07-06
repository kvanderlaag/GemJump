#include <levels.h>
#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <string.h>
#include <values.h>

level load_level(char* filename) {
    level loadedLevel;
    initialize_level(&loadedLevel);
    FILE* levelFile = NULL;

    char readChar;
    int magic = 0;

    // Open File
    levelFile = fopen(filename, "r");
    if (levelFile == NULL) {
        fprintf(stderr, "Could not open file %s\n", filename);
        loadedLevel.name = NULL;
        return loadedLevel;
    }

    // Check for magic numbers

    for (int i = 0; i < 3; i++) {
        magic += getc(levelFile);
    }

    if (magic != 14) {
        fprintf(stderr, "Invalid level file: %s\n", filename);
        loadedLevel.name = NULL;
        return loadedLevel;
    }

    // Read number of rows and columns
    loadedLevel.rows = getc(levelFile);
    loadedLevel.cols = getc(levelFile);

    // Read Time Limit
    loadedLevel.time = getc(levelFile);

    char name[MAX_NAME_LEN];
    int nameCount = 0;
    while ((readChar = getc(levelFile)) != '\0') {
        name[nameCount] = readChar;
        nameCount++;
    }
    loadedLevel.name = malloc(sizeof(char) * strlen(&name[0]));
    strcpy(loadedLevel.name, &name[0]);
        //fprintf(stderr, "%s", loadedLevel.name);
        //scanf(">");

    readChar = getc(levelFile);
    while (readChar != EOF) {

        struct entity* ent = malloc(sizeof(struct entity));
        ent->type = readChar;
        ent->row = getc(levelFile);
        ent->col = getc(levelFile);
        switch (ent->type) {
        case (0x02):
            loadedLevel.gems++;
            break;
        case (0x03):
            loadedLevel.playerStartCol = ent->col;
            loadedLevel.playerStartRow = ent->row;
            break;
        }
        ent->length = getc(levelFile);
        ent->next = loadedLevel.entityList;
        loadedLevel.entityList = ent;
        readChar = getc(levelFile);
    }

    loadedLevel.viewData = malloc(sizeof(char) * loadedLevel.rows * loadedLevel.cols);
    char* viewPointer = &loadedLevel.viewData[0];
    struct entity* ent = loadedLevel.entityList;

    while (ent != NULL) {
        viewPointer = loadedLevel.viewData + (ent->row * loadedLevel.cols) + ent->col;
        switch (ent->type) {
        case 0x01:
            for (int i = 0; i < ent->length; i++)
                    viewPointer[i] = PLATFORM_CH;
            break;
        case 0x02:
            *(viewPointer) = GEM_CH;
            break;
        case 0x03:
            *(viewPointer) = PLAYER_CH;
            break;
        }
        ent = ent->next;
    }



    fclose(levelFile);

    return loadedLevel;
}

/*
    void print_level_data

    Diagnostic. Prints level name, rows, columns, and time limit.

*/
void print_level_data(level* activeLevel, WINDOW* win) {
    mvwprintw(win, 0, 0, "Name: %s", activeLevel->name);
    mvwprintw(win, 6, 0, "Name Length: %d", strlen(activeLevel->name));
    mvwprintw(win, 1, 0, "Rows: %d", activeLevel->rows);
    mvwprintw(win, 2, 0, "Columns: %d", activeLevel->cols);
    mvwprintw(win, 3, 0, "Time Limit: %d", activeLevel->time);
    mvwprintw(win, 4, 0, "Gems: %d", activeLevel->gems);
    mvwprintw(win, 5, 0, "Player Start: (%d,%d)", activeLevel->playerStartRow, activeLevel->playerStartCol);
    struct entity* ent = activeLevel->entityList;
    int i = 0;
    while (ent != NULL) {
        wmove(win, 8 + (2*i), 0);
        switch (ent->type) {
            case 0x01:
                wprintw(win, "Platform - Row: %d, Col: %d, Length: %d", ent->row, ent->col, ent->length);
                break;
            case 0x02:
                wprintw(win, "Gem - Row: %d, Col: %d", ent->row, ent->col);
                break;
        }
        ent = ent->next;
        i++;
    }

    return;
}


/*
    void display_level

    Display active level in specified window, starting from row/column
    offset passed to function
*/
void display_level(level* activeLevel, int row, int col, WINDOW* win) {
    int i = 0, j = 0;
    while ((i < (getmaxy(win) - 2)) && (i < activeLevel->rows)) {
        j = 0;
        wmove(win, i + 1, 1);
        while ((j < (getmaxx(win) - 2)) && (j < activeLevel->cols)) {
            unsigned char found = *(activeLevel->viewData + ((i + row)* activeLevel->cols) + j + col);
            switch (found) {
                case PLATFORM_CH:
                    waddch(win, PLATFORM_CH);
                    break;
                /*case PLAYER_CH:
                    waddch(win, PLAYER_CH);
                    break;*/
                case GEM_CH:
                    waddch(win, ACS_DIAMOND);
                    break;
                default:
                    waddch(win, ' ');
            }
            j++;
        }
        i++;
    }
    box(win, 0, 0);
    mvwprintw(stdscr, 0, (getmaxx(stdscr) / 2) - (strlen(activeLevel->name) / 2), "%s", activeLevel->name );
    return;
}


void initialize_level(level* activeLevel) {
    activeLevel->name = "Untitled Level";
    activeLevel->cols = 0;
    activeLevel->rows = 0;
    activeLevel->playerStartCol = 0;
    activeLevel->playerStartRow = 0;
    activeLevel->time = 0;
    activeLevel->gems = 0;
    activeLevel->entityList = NULL;
    activeLevel->viewData = NULL;

}
