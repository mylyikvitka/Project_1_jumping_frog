#include <ncurses.h>
#include "fun/fun.h"
#include <cstdlib>
#include <ctime>
<<<<<<< HEAD


=======
#define HEIGTH 15
#define WIDTH HEIGTH*2
#include <unistd.h>
>>>>>>> eff9e6943bb542bca43cc4a272a95e52a3c4eb78


int main() {
    // Initialize ncurses
    initscr();
    refresh();
    noecho();
    start_color();
<<<<<<< HEAD
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_CYAN, COLOR_BLACK);
    init_pair(3, COLOR_WHITE, COLOR_BLACK);
    init_pair(4, COLOR_RED, COLOR_BLACK);
    init_pair(5, COLOR_YELLOW, COLOR_BLACK);
    init_pair(6, COLOR_MAGENTA, COLOR_BLACK);
    srand(static_cast<unsigned>(time(0)));
    Config config = readConfig("game_config.txt");

    bkgd(COLOR_PAIR(3));
    clear();
    WINDOW * window =drawInterface(config);
    frogGame(window);
=======
    // Start color functionality
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    init_pair(2, COLOR_CYAN, COLOR_BLACK);
    srand(static_cast<unsigned>(time(0)));
    init_pair(3, COLOR_WHITE, COLOR_BLACK);  // White text on black background
    init_pair(4, COLOR_RED, COLOR_BLACK);
    init_pair(5, COLOR_YELLOW, COLOR_BLACK);
    init_pair(6, COLOR_MAGENTA, COLOR_BLACK);

    // Set the background color to black
    bkgd(COLOR_PAIR(3));  // Apply color pair 1 for the entire window
    clear();


    WINDOW * window =drawInterface();

    frogGame(window);
    // Refresh and wait for user input


    // End ncurses mode
>>>>>>> eff9e6943bb542bca43cc4a272a95e52a3c4eb78
    endwin();
    return 0;
}
