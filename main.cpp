#include <ncurses.h>
#include "fun/fun.h"
#include <cstdlib>
#include <ctime>
#define HEIGTH 15
#define WIDTH HEIGTH*2
#include <unistd.h>


int main() {
    // Initialize ncurses
    initscr();
    refresh();
    noecho();
    start_color();
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
    endwin();
    return 0;
}