//
//
//


#ifndef FUN_H
#define FUN_H
#include <ncurses.h>
<<<<<<< HEAD
#include <ctime>

=======
#include <unistd.h>
>>>>>>> eff9e6943bb542bca43cc4a272a95e52a3c4eb78


struct Frog {
    int x;
    int y;
    chtype symbol;
<<<<<<< HEAD
    bool inTheCar=false;
    clock_t last_move_time = 0;

=======
>>>>>>> eff9e6943bb542bca43cc4a272a95e52a3c4eb78
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
<<<<<<< HEAD
    bool hasFrog=false;
=======
>>>>>>> eff9e6943bb542bca43cc4a272a95e52a3c4eb78
};


struct Timer {
    unsigned int frame_time;
    float pass_time;
    int frame_no;
};

struct Obstacle {
<<<<<<< HEAD
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

=======
    int x, y;      // Position of the obstacle
    chtype symbol; // Symbol representing the obstacle
};

WINDOW* drawInterface();
chtype getInput(WINDOW * win);
void refreshBoard(WINDOW * win);
void addAt(int y, int x, chtype ch);
void frogMove(WINDOW* win, Frog& frog, chtype input);
int frogGame(WINDOW * win);
>>>>>>> eff9e6943bb542bca43cc4a272a95e52a3c4eb78






#endif //FUN_H

