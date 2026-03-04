#include <ncurses.h>
#include "fun/fun.h"
#include <cstdlib>
#include <ctime>




int main() {
    // Initialize ncurses
    initscr();
    refresh();
    noecho();
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_CYAN, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    init_pair(4, COLOR_RED, COLOR_BLACK);
    init_pair(5, COLOR_YELLOW, COLOR_BLACK);
    init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
    srand(static_cast<unsigned>(time(0)));
    Config config = readConfig("/Users/miawwww/BasComp2425/Project_1_jumping_frog/game_config.txt");

    bkgd(COLOR_PAIR(3));
    clear();
    WINDOW * window =drawInterface(config);
    frogGame(window);
    endwin();
    return 0;
}