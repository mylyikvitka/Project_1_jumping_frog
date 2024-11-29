#include "fun.h"
#include <ncurses.h>
#include <unistd.h>
#include <ctime>
#include<cstdlib>
#include <cmath>
#include<stdio.h>
//actuall 24
#define HEIGTH 24
//actuall 48
#define WIDTH 48
#define LANE_HEIGTH 4
#define NUM_LANES 5
#define NUM_CARS 5
#define NUM_OBSTACLES 10


// DRAWING THE BOX SHAPE
WINDOW* drawInterface() {
   int maxY, maxX;
   getmaxyx(stdscr, maxY, maxX);
   int startY = (maxY / 2) - ((HEIGTH) / 2);
   int startX = (maxX / 2) - ((WIDTH) / 2);

   // Create the game window
   WINDOW* window = newwin(HEIGTH, WIDTH, startY, startX);
   refresh();  // Refresh the main screen before drawing
   // Draw game box
    box(window,0,0);
   wrefresh(window);


   mvprintw(1, 0, "Ivanytskyi");
   mvprintw(2, 1, " Dmytro");
   mvprintw(3, 1, "s201251");
   // Adjust position as needed
   refresh();  // Refresh the main screen to display the signature




   // Add signature above the box
   return window;
}




chtype getInput(WINDOW* win) {
   return wgetch(win);
}


void refreshBoard(WINDOW* win) {
   wrefresh(win);
}


void addAt(WINDOW* win, int y, int x, chtype ch) {
   mvwaddch(win, y, x, ch);
}


void clearFrog(WINDOW* win, const Frog& frog) {
   mvwprintw(win, frog.y, frog.x, "  ");
   wrefresh(win);
}


void drawFrog(WINDOW* win, const Frog& frog) {
    if(!frog.inTheCar) {
        wcolor_set(win, 1, NULL);  // Set color to green (color pair 1)
        mvwprintw(win, frog.y, frog.x, "@");
        wrefresh(win);
        wcolor_set(win, 0, NULL);
    }// Reset color to default
}

void drawObstacles(WINDOW* win, const Obstacle obstacles[], int num_obstacles) {
    for (int i = 0; i < num_obstacles; ++i) {
        wcolor_set(win, 4, NULL);
        mvwaddch(win, obstacles[i].y, obstacles[i].x, obstacles[i].symbol);
        wcolor_set(win, 0, NULL);
    }
    wrefresh(win);
    // Refresh the window to display obstacles
}



void drawCar(WINDOW* win, const Car& car) {
    if (car.hasFrog) {
        wcolor_set(win, 1, NULL); // Set color to green
    } else {
        switch (car.type) {
            case 0: wcolor_set(win, 2, NULL); break;
            case 1: wcolor_set(win, 5, NULL); break;
            case 2: wcolor_set(win, 6, NULL); break;
        }
    }

    mvwprintw(win, car.y, car.x, "#");
    wrefresh(win);
    wcolor_set(win, 0, NULL); // Reset color to default
}


void initializeCars(Car cars[], int num_cars) {

   for (int i = 0; i < num_cars; ++i) {
       int pos = i * LANE_HEIGTH + 2;


       // Set vertical position of the car
       cars[i].y = pos + 1;


       // Set random horizontal position
       cars[i].x = WIDTH - 2;  // Off-screen at the start, with random offset


       // Assign car symbol
       cars[i].symbol = '#';


       // Set random move interval (speed)
       cars[i].move_interval = 3 + rand() % 6;


       // Set when the car should spawn (randomly staggered)
       cars[i].spawn_time = rand() % 20;  // Random spawn time between 0 and 19 frames


       cars[i].on_field = true;  // Initialize as visible
       cars[i].respawn_time = 0;


       // Initialize other properties
       cars[i].last_move_frame = 0;
       cars[i].type = rand() % 2;
       if (cars[i].type == 1) {
           cars[i].type = (rand() % 2 == 0) ? 2 : 1;
       }

       }
}


void changeCarSpeed(Car cars[], int num_cars, int timer_frame_no) {
    // Randomly select a car to change speed
    int carIndex = rand() % num_cars;

    // Change its speed every 100 frames
    if (timer_frame_no % 100 == 0) {
        int newSpeed = 2 + rand() % 8; // Random new speed
        cars[carIndex].move_interval = newSpeed;
    }
}

bool isCloseToFrog(Car& car, int frogX, int frogY) {
    // If the frog is in front of the car (frog.x < car.x, i.e., frog is left of the car)
    if (frogX < car.x) {
        // Only stop if the frog is close in the y-direction and sufficiently ahead in x-direction
        return (abs(car.y - frogY) <= 1 && (car.x - frogX) < 8);  // Stop if frog is within y-range and ahead in x
    }
    // If the frog is behind the car (frog.x > car.x, i.e., frog is right of the car)
    else {
        // Only stop if the frog is close in the y-direction and near in the x-direction
        return (abs(car.y - frogY) <= 1 && (frogX - car.x) < 2);  // Stop if frog is behind and near in x
    }
}


void moveCars(WINDOW* win, Car cars[], int num_cars, Timer& timer, Frog frog) {
   for (int i = 0; i < num_cars; ++i) {
       // Handle reappearing of cars that are not on the field
       if (!cars[i].on_field && timer.frame_no >= cars[i].respawn_time) {
           // Reinitialize the car
           cars[i].x = WIDTH - 2;  // Reset position to the right side
           cars[i].y = (i * LANE_HEIGTH + 2) + 1;  // Assign lane position
           cars[i].symbol = '#';
           cars[i].move_interval = 3 + rand() % 6;  // Randomize move interval
           cars[i].spawn_time = rand() % 20;  // Reset spawn time
           cars[i].type = rand() % 2;
           if (cars[i].type == 1) {
               cars[i].type = (rand() % 2 == 0) ? 2 : 1;
           }// Randomize type
           cars[i].on_field = true;  // Set car as visible
           cars[i].last_move_frame = 0;  // Reset movement frame
       }


       // Skip if the car is currently not on the field
       if (!cars[i].on_field) {
           continue;
       }


       // Clear previous position of the car (if it has moved)
       mvwaddch(win, cars[i].y, cars[i].x, ' ');
       // Check if the car is friendly and close to the frog
       if ((cars[i].type == 1 ||cars[i].type==2)&& isCloseToFrog(cars[i], frog.x, frog.y)) {
           // Skip movement for friendly car if it's close to the frog
           drawCar(win, cars[i]);
           continue;
       }



       if (timer.frame_no - cars[i].last_move_frame >= cars[i].move_interval) {
           cars[i].x -= 1;  // Move the car left
           cars[i].last_move_frame = timer.frame_no;
       }


       // Draw the car at its new position
       drawCar(win, cars[i]);


       // Check if the car has reached the left border
       if (cars[i].x == 1) {
           // Random chance for the car to disappear
           if (rand() % 2 == 0) {  // 50% chance to disappear
               cars[i].on_field = false;
               cars[i].respawn_time = timer.frame_no + (rand() % 50 + 80);  // Respawn after random frames
           } else {
               cars[i].x = WIDTH - 2;  // Otherwise, reset to the right side
           }
       }
   }
}




void drawLanes(WINDOW* win) {
   for (int x = 1; x < WIDTH - 1; ++x) {  // Keep inside box borders
       wcolor_set(win, 1, NULL);  // Set color to green (color pair 3)
       mvwaddch(win, 1, x, '.');  // Draw the "win" area in green
   }
   wcolor_set(win, 0, NULL);
   for (int i = 0; i < NUM_LANES; ++i) {  // Draw 5 lanes
       int y = i * LANE_HEIGTH + 2;  // Calculate the y position for each lane (1-based)
       for (int x = 1; x < WIDTH - 1; ++x) {  // Keep inside box borders
           mvwaddch(win, y, x, '-');
           mvwaddch(win, y + 1, x, '-');  // Each lane is 3 units high
           mvwaddch(win, y + 2, x, '-');
       }
   }
   wrefresh(win); // Refresh to show lanes on screen
}


void collisionF(Car cars[], Frog& frog, bool &collision) {
   for (int i = 0; i < NUM_CARS; ++i) {
       // Check if any car is in the same vertical range as the frog
       if(cars[i].type==0){
       if (cars[i].x == frog.x && cars[i].y == frog.y) {
           // Collision detected, car hits frog
           collision = true;
       }

   }
       // Check if car is about to "pass" over the frog


   }
}


void endGame(WINDOW *win,bool collision,bool victory, bool &gameOver, chtype input) {


    if(victory==true) {
       mvwprintw(win, HEIGTH / 2, WIDTH / 2 - 5, "YOU WON!");
       wrefresh(win);
       usleep(2000000);
       gameOver = true;
   }


    if (input=='q'||input=='Q') {
       mvwprintw(win, HEIGTH / 2, WIDTH / 2 - 5, "GAME OVER!");
       wrefresh(win);
       usleep(2000000);
       gameOver = true;
   }


  if (collision==true){
       mvwprintw(win, HEIGTH / 2, WIDTH / 2 - 5, "GAME OVER!");
       wrefresh(win);
       usleep(2000000);
       gameOver = true;
   }




}


bool isObstacleAt(int x, int y, const Obstacle obstacles[], int num_obstacles, const Car cars[], int num_cars) {
    // Check if there's an obstacle in the obstacles array
    for (int i = 0; i < num_obstacles; ++i) {
        if (obstacles[i].x == x && obstacles[i].y == y) {
            return true;  // There is an obstacle at the given position
        }
    }

    // Check if a car is at the given position and its type is 1 or 2 (obstacle)
    for (int i = 0; i < num_cars; ++i) {
        if (cars[i].x == x && cars[i].y == y && (cars[i].type == 1 || cars[i].type == 2)) {
            return true;  // There is a car (type 1 or 2) at the given position, treat as an obstacle
        }
    }

    return false;  // No obstacle at the given position
}

void hopInCar(WINDOW* win, Frog& frog, chtype input, Car cars[], int num_cars) {
    // Check if the input is 'e'
    if (input == 'e') {
        for (int i = 0; i < num_cars; ++i) {
            // Check if the car is type 2 and the frog is close enough
            if (cars[i].type == 2 && abs(cars[i].x - frog.x) <= 1 && abs(cars[i].y - frog.y) <= 1) {
                // Update frog position to the car's position
                frog.x = WIDTH/2;
                frog.y = HEIGTH-2;

                // Mark the car as occupied
                cars[i].hasFrog = true;
                frog.inTheCar=true;

                // Redraw the car
                drawCar(win, cars[i]);

                // Erase the frog from the screen (optional since it's now "inside" the car)
                mvwaddch(win, frog.y, frog.x, ' ');

                return; // Stop checking other cars
            }
        }
    }
}


void frogMove(WINDOW* win, Frog& frog, chtype input, bool &victory, const Obstacle obstacles[], int num_obstacles,const Car cars[]) {
    clearFrog(win, frog);  // Use the Frog struct directly

    // Temporary coordinates for the potential new position
    int new_x = frog.x;
    int new_y = frog.y;

    if (input == 'w' && frog.y > 1) {
        new_y--;
        if (new_y == 1) {
            victory = true;
        }
    } else if (input == 'a' && frog.x > 1) {
        new_x--;
    } else if (input == 's' && frog.y < HEIGTH - 2) {
        new_y++;
    } else if (input == 'd' && frog.x < WIDTH - 3) {
        new_x++;
    }

    // Check if the new position is free of obstacles
    if (!isObstacleAt(new_x, new_y, obstacles, num_obstacles,cars,NUM_CARS)) {
        frog.x = new_x;
        frog.y = new_y;
    }

    drawFrog(win, frog);  // Use the Frog struct directly
}



void displayTimer(WINDOW* win, const Timer& timer) {
   // Print the timer information on the screen
   // Position it somewhere outside the main game box if possible
   mvprintw(5, 0, "Time: %d", timer.frame_no);// Adjust position as needed
   refresh();
}



void drawBorders(WINDOW* win) {
    // Draw the borders (top, bottom, left, right)
    box(win, 0, 0);  // Draw a box around the window (using default characters)
}
void displayRealTimeTimer(WINDOW* win, clock_t start_time) {
    // Calculate the elapsed time in seconds
    double elapsed_seconds = double(clock() - start_time) / CLOCKS_PER_SEC;

    // Calculate minutes and whole seconds
    int minutes = static_cast<int>(elapsed_seconds) / 60; // Full minutes
    int seconds = static_cast<int>(elapsed_seconds) % 60; // Remainder as seconds

    // Print the elapsed time on the screen in minutes:seconds format
    mvprintw(9, 0, "Time: %d:%02d", minutes, seconds); // Ensure two digits for seconds
    refresh();
}

void initializeObstacles(Obstacle obstacles[], int num_obstacles, int num_lanes, int lane_height, Car car[]) {
    for (int i = 0; i < num_obstacles; ++i) {
        bool validPosition = false;

        // Use the original idea for 'pos'
        obstacles[i].x = 2 + rand() % (WIDTH - 4);

        while (!validPosition) {
            // Randomize y position for the obstacle
            obstacles[i].y = 2 + rand() % (HEIGTH - 4);

            // Check that the obstacle does not overlap with any car's y position
            validPosition = true;  // Assume the position is valid

            for (int j = 0; j < NUM_CARS; j++) {
                if (obstacles[i].y == car[j].y) {
                    validPosition = false;  // If it matches any car's y position, set invalid
                    break;  // No need to check further, as we've found an overlap
                }
            }
        }

        obstacles[i].symbol = '*';  // Use '*' as the obstacle symbol
    }
}







int frogGame(WINDOW* win) {
    bool gameOver = false;
    bool victory = false;
    bool collision = false;
    bool inCar=false;
    Obstacle obstacles[NUM_OBSTACLES];
    Frog frog = {WIDTH / 2, HEIGTH - 2, '@'};
    Car cars[NUM_CARS];  // Array of cars
    Timer timer = {100, 0.0f, 0};
    initializeCars(cars, NUM_CARS);
    initializeObstacles(obstacles, NUM_OBSTACLES, NUM_LANES, LANE_HEIGTH, cars);
    clock_t start_time = clock();

    drawLanes(win);  // Draw lanes initially
    drawFrog(win, frog);
    drawObstacles(win, obstacles, NUM_OBSTACLES);// Draw frog initially

    // Enable non-blocking input
    nodelay(win, TRUE);  // Non-blocking input (no wait for key press)

    while (!gameOver) {
        // Clear the window for the new frame (without erasing borders)
       // Clears the window content

        // Redraw the borders of the game window

        // Redraw lanes and frog
        drawLanes(win);

        drawFrog(win, frog);
        drawObstacles(win, obstacles, NUM_OBSTACLES);// Draw frog initially




        // Get input (non-blocking)
        chtype input = getInput(win);

        // Handle input if available
        if (input != ERR) {
            frogMove(win, frog, input, victory, obstacles, NUM_OBSTACLES,cars);
            hopInCar(win, frog, input, cars, NUM_CARS);


            // Move frog based on input
            collisionF(cars, frog, collision);
            // Check if frog collides with a car
        }

        // Move and draw cars
        moveCars(win, cars, NUM_CARS, timer,frog);
        changeCarSpeed(cars, NUM_CARS, timer.frame_no);// Move cars and draw them

        // Check collision and handle game over/victory
        collisionF(cars, frog, collision);
        endGame(win, collision, victory, gameOver, input);

        // Display timer
        displayRealTimeTimer(win, start_time);


        // Update frame number and pass time
        timer.frame_no++;
        timer.pass_time += timer.frame_time / 1000.0f;

        // Small delay to smooth the game's speed
        usleep(10000);  // Delay for 10 milliseconds

        drawBorders(win);
        // Refresh the screen to show the updated positions
        wrefresh(win);
    }

    return gameOver;
}

