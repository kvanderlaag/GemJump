#include <cleanup.h>
#include <curses.h>
#include <string.h>

int cleanup() {
    // End curses mode.
    endwin();
    return 0;
}


void winlose(bool winning, int time, int gems) {
    erase();

    if (winning) {
        mvprintw( (getmaxy(stdscr) / 2), (getmaxx(stdscr) / 2) - (strlen("You Won!") / 2), "You Won!" );
        mvprintw( (getmaxy(stdscr) / 2) + 1, (getmaxx(stdscr) / 2) - (strlen("(    seconds left.") / 2), "(%03d seconds left.)", time );
    } else {
        mvprintw( (getmaxy(stdscr) / 2), (getmaxx(stdscr) / 2) - (strlen("You Lost!") / 2), "You Lost!" );
        mvprintw( (getmaxy(stdscr) / 2) + 1, (getmaxx(stdscr) / 2) - (strlen("(    gems left.") / 2), "(%03d gems left.)", gems );
    }
    mvprintw( (getmaxy(stdscr) / 2) + 2, (getmaxx(stdscr) / 2) - (strlen("Press 'q' key to exit.") / 2), "Press 'q' key to exit.");

    while (getch() != 'q');
}
