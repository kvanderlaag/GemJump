#include <initialize.h>
#include <curses.h>
#include <string.h>
#include <values.h>

int initialize() {

    // Set up curses mode.
    initscr();
    raw();
    curs_set(0);
    nodelay(stdscr, TRUE); // No delay when waiting for keyboard input.
    keypad(stdscr, TRUE);
    noecho();

    // No errors, return 0.
    return 0;
}

int instructions() {
    int startrow, startcol;
    startcol = (getmaxx(stdscr) / 2) - (strlen(INSTR_STR1) / 2);
    startrow = (getmaxy(stdscr) / 2) - ((INSTR_STR_NUM + 2) / 2);
    mvprintw(startrow, startcol, INSTR_STR1);

    startcol = (getmaxx(stdscr) / 2) - (strlen(INSTR_STR2) / 2);
    startrow += 2;
    mvprintw(startrow++, startcol, INSTR_STR2);
    startcol = (getmaxx(stdscr) / 2) - (strlen(INSTR_STR5) / 2);
    mvprintw(startrow++, startcol, INSTR_STR3);
    mvprintw(startrow++, startcol, INSTR_STR4);
    mvprintw(startrow++, startcol, INSTR_STR5);
    mvprintw(startrow++, startcol, INSTR_STR6);
    startrow += 1;
    startcol = (getmaxx(stdscr) / 2) - (strlen(INSTR_STR7) / 2);
    mvprintw(startrow, startcol, INSTR_STR7);

    while (getch() == ERR);
    erase();
}
