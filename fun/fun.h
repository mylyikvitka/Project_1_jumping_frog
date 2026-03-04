//
// Created by Dima Ivanytskyi on 11/7/24.
//


#ifndef FUN_H
#define FUN_H
#include <ncurses.h>
#include <ctime>



struct Frog {
    int x;
    int y;
    chtype symbol;
    bool inTheCar=false;
    clock_t last_move_time = 0;

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
    int x, y;
    chtype symbol;
};

struct Config {
    int height;
    int width;
    int lane_height;
    int num_lanes;
    int num_cars;
    int num_obstacles;
    chtype symbolFrog;
    chtype symbolCar;
    chtype symbolObstacle;
};

WINDOW* drawInterface(const Config& config);

void frogMove(WINDOW* win, Frog& frog, chtype input);
int frogGame(WINDOW * win);
Config readConfig(const char *filename);
void hopInCar(WINDOW* win, Frog& frog, chtype input, Car cars[], const Config& config);







#endif //FUN_H

