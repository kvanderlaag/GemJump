#include <curses.h>
#include <string.h>

void update_hud(WINDOW* win, int gems, int time) {
    wmove(win, 0, 0);
    waddch(win, '+');
    for (int i = 1; i < getmaxx(stdscr) - 1; i++) {
        waddch(win, '-');
    }
    waddch(win, '+');
    wmove(win, 1, 0);
    wprintw(win, "Gems Left: %d | Time Left: %d  ", gems, time);
    //"Time Left: "
}
