//
// Created by Dima Ivanytskyi on 11/7/24.
//


#ifndef FUN_H
#define FUN_H
#include <ncurses.h>
#include <unistd.h>


struct Frog {
    int x;
    int y;
    chtype symbol;
    bool inTheCar=false;
};


struct Car {
    int x;
    int y;
    chtype symbol;
    int move_interval; // interval to move the car (in frames)
    int last_move_frame;
    int direction;
    int spawn_time;
    bool on_field;
    int respawn_time;
    int type;
    bool hasFrog=false;
};


struct Timer {
    unsigned int frame_time;
    float pass_time;
    int frame_no;
};

struct Obstacle {
    int x, y;      // Position of the obstacle
    chtype symbol; // Symbol representing the obstacle
};

WINDOW* drawInterface();
chtype getInput(WINDOW * win);
void refreshBoard(WINDOW * win);
void addAt(int y, int x, chtype ch);
void frogMove(WINDOW* win, Frog& frog, chtype input);
int frogGame(WINDOW * win);






#endif //FUN_H

