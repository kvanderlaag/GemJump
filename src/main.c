#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include <initialize.h>
#include <cleanup.h>
#include <levels.h>
#include <values.h>
#include <hud.h>
#include <player.h>
#include <time.h>


int main(int argc, char* argv[]) {
    // Run initialization functions.
    initialize();

    char* filename;
    if (argc >= 2) {
        filename = malloc(sizeof(argv[1]));
        strcpy(filename, argv[1]);
    } else {
        filename = "level1.d";
    }
    // Load a level
    level activeLevel = load_level(filename);
    if (activeLevel.name == NULL) {
        fprintf(stderr, "\nError: Could not load level file %s. Exiting.", filename);
        exit(-1);
    }

    instructions();

    // Diagnostic: Print Level Data
    // print_level_data(activeLevel, stdscr);
    // while (getch() != 'q');

    // Set up Windows
    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    if (activeLevel.cols < cols) {
        cols = activeLevel.cols + 2;
    }
    if (activeLevel.rows < rows - 2) {
        rows = activeLevel.rows + 2;
    } else {
        rows = rows - 2;
    }

    int startrow = ((getmaxy(stdscr) - 2) / 2) - (rows / 2) + 1;
    int startcol = ((getmaxx(stdscr) - 2) / 2) - (cols / 2);

    WINDOW* field = newwin(rows, cols, startrow, startcol);
    WINDOW* hud = newwin(2, getmaxx(stdscr), getmaxy(stdscr)-2, 0);
    refresh();

    /*
    print_level_data(&activeLevel, stdscr);
    refresh();
    while(getch() != 'q');
    */



    int inputCh;
    player currentPlayer = init_player(&activeLevel);
    int millis = 0;
    int inputMillis = 0;
    int timeMillis = 0;
    clock_t begin, end;
    bool running = TRUE;
    bool winning = FALSE;

    while (running) {
        begin = clock();
        // Display level on screen
        display_level(&activeLevel, 0, 0, field);
        display_player(&currentPlayer, &activeLevel, field , 0, 0);
        update_hud(hud, activeLevel.gems, activeLevel.time);
        if (is_wintouched(hud)) {
            wrefresh(hud);
        }
        if (is_wintouched(field)) {
            wrefresh(field);
        }

        check_gems(&currentPlayer, &activeLevel);
        if (activeLevel.gems == 0) {
            winning = TRUE;
            running = FALSE;
        }

        if (inputMillis == 0) {
            inputCh = getch();
            switch (inputCh) {
                case KEY_LEFT:
                    move_player(&currentPlayer, &activeLevel, 'l');
                    inputMillis = INPUT_INT;
                    break;
                case KEY_RIGHT:
                    move_player(&currentPlayer, &activeLevel, 'r');
                    inputMillis = INPUT_INT;
                    break;
                case ' ':
                    jump(&currentPlayer, &activeLevel);
                    inputMillis = INPUT_INT;
                    break;
                case 'q':
                    running = FALSE;
                    break;
            }
        } else {
            flushinp();
        }

        end = clock();
        millis = millis + (end - begin);

        if (millis >= GRAVITY_INT) {
            check_gravity(&currentPlayer, &activeLevel);
            millis = 0;
        }

        timeMillis += (end - begin);

        if (timeMillis >= 1000) {
            activeLevel.time--;
            if (activeLevel.time == 0) {
                // End game code here
                running = FALSE;
            }
            timeMillis = 0;
        }

        if (inputMillis >= 0) {
            inputMillis -= (end - begin);
            if (inputMillis < 0)
                inputMillis = 0;
        }
    }

    delwin(field);
    delwin(hud);

    winlose(winning, activeLevel.time, activeLevel.gems);

    // Done, clean up.
    cleanup();
    return 0;
}
