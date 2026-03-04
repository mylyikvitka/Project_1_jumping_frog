#include "fun.h"
#include <ncurses.h>
#include <unistd.h>
#include <ctime>
#include <cmath>
#include <cstdio>
#include <cstring>
#define BASE_SCORE 100
#define MOVE_PENALTY 1
#define COLLISION_PENALTY 5
#define TIME_PENALTY 5
#define TIME_INTERVAL 10
#define MOVE_CAP 12
#define MAX_FRAME 20
#define ENEMY 0
#define NEUTRAL 1
#define FRIENDLY 2
#define SPEED_INTERVAL 100
#define ESC 27 // ASCII for 'esc'
#define QUIT 113 // ASCII for 'q'
#define INTERACT 101
#define GO_UP 119   // ASCII for 'w'
#define GO_LEFT 97    // ASCII for 'a'
#define GO_DOWN 115   // ASCII for 's'
#define GO_RIGHT 100  // ASCII for 'd'
#define PRINT 112   // ASCII for 'p'
#define LOAD 108 // ASCII for 'l'


//CONFIGURATION
Config readConfig(const char *filename) {
  Config config = {24, 48, 4, 5, 5, 10}; // Default values


  FILE *file = fopen(filename, "r");


  char key[20];
    int value;
  char symbol[2];




  while (fscanf(file, "%[^=]=%s\n", key, symbol) == 2) {
      if (strcmp(key, "HEIGHT") == 0) {
          config.height = atoi(symbol);
      } else if (strcmp(key, "WIDTH") == 0) {
          config.width = atoi(symbol);
      } else if (strcmp(key, "LANE_HEIGHT") == 0) {
          config.lane_height = atoi(symbol);
      } else if (strcmp(key, "NUM_LANES") == 0) {
          config.num_lanes = atoi(symbol);
      } else if (strcmp(key, "NUM_CARS") == 0) {
          config.num_cars = atoi(symbol);
      } else if (strcmp(key, "NUM_OBSTACLES") == 0) {
          config.num_obstacles = atoi(symbol);
      }
      else if (strcmp(key, "SYMBOL_FROG") == 0) {
          config.symbolFrog = symbol[0];
      }
      else if (strcmp(key, "SYMBOL_CAR") == 0) {
          config.symbolCar = symbol[0];
      }
      else if (strcmp(key, "SYMBOL_OBSTACLE") == 0) {
          config.symbolObstacle = symbol[0];
      }
  }




  fclose(file);
  return config;
}




void saveGame(const char* filename, const Frog& frog, const Car cars[], const Config& config,int moveCounter,int collisionCounter) {
   FILE* file = fopen(filename, "w");
   if (!file) {
       perror("Error opening save file");
       return;
   }

   fprintf(file, "FROG_X=%d\n", frog.x);
   fprintf(file, "FROG_Y=%d\n", frog.y);
   fprintf(file, "FROG_IN_THE_CAR=%d\n", frog.inTheCar ? 1 : 0);

   for (int i = 0; i < config.num_cars; ++i) {
       fprintf(file, "CAR_%d_X=%d\n", i, cars[i].x);
       fprintf(file, "CAR_%d_Y=%d\n", i, cars[i].y);
       fprintf(file, "CAR_%d_ON_FIELD=%d\n", i, cars[i].on_field ? 1 : 0);
       fprintf(file, "CAR_%d_TYPE=%d\n", i, cars[i].type);
       fprintf(file, "CAR_%d_MOVE_INTERVAL=%d\n", i, cars[i].move_interval);
       fprintf(file, "CAR_%d_HAS_FROG=%d\n", i, cars[i].hasFrog ? 1 : 0);
   }
   fprintf(file, "MOVE_COUNTER=%d\n", moveCounter);
   fprintf(file, "COLLISION_COUNTER=%d\n", collisionCounter);


   fclose(file);
}




//                                                                    DRAWING FUNCTIONS
WINDOW* drawInterface(const Config& config) {
 int maxY, maxX;
 getmaxyx(stdscr, maxY, maxX);
 int startY = (maxY / 2) - (config.height / 2);
 int startX = (maxX / 2) - (config.width / 2);


 // Create the game window
 WINDOW* window = newwin(config.height, config.width, startY, startX);
 refresh();
 box(window, 0, 0);
 wrefresh(window);
 mvprintw(1, 0, "Ivanytskyi");
 mvprintw(2, 1, " Dmytro");
 mvprintw(3, 1, "s201251");
  mvprintw(5, 1, "Status");
  mvprintw(6, 1, "Active");
 refresh();


 return window;
}




void displayMoveCounter(int moveCounter) {
  mvprintw(12, 1, "Moves:");
  mvprintw(13, 1, "%d", moveCounter);
  refresh();
}




void drawFrog(WINDOW* win, const Frog& frog, const Config& config) {
//draw the frog unless it's in the car
    if (!frog.inTheCar) {
      wcolor_set(win, 1, NULL);
      mvwprintw(win, frog.y, frog.x, "%c", config.symbolFrog);
      wrefresh(win);
      wcolor_set(win, 0, NULL);
  }
}




void clearFrog(WINDOW* win, const Frog& frog) {
  mvwprintw(win, frog.y, frog.x, "  ");
  wrefresh(win);
}








void drawObstacles(WINDOW* win, const Obstacle obstacles[], const Config& config) {
  for (int i = 0; i < config.num_obstacles; ++i) {
      wcolor_set(win, 4, NULL);
      mvwaddch(win, obstacles[i].y, obstacles[i].x, obstacles[i].symbol);
      wcolor_set(win, 0, NULL);
  }
  wrefresh(win);
}






void drawCar(WINDOW* win, const Car& car, const Config& config) {
 if (car.hasFrog) {
     wcolor_set(win, 1, NULL); // Set color to green (occupied)
 } else {
     switch (car.type) {
         case ENEMY: wcolor_set(win, 2, NULL); break;
         case NEUTRAL: wcolor_set(win, 5, NULL); break;
         case FRIENDLY: wcolor_set(win, 6, NULL); break;
     }
 }
 mvwprintw(win, car.y, car.x, "%c", config.symbolCar);
 wrefresh(win);
 wcolor_set(win, 0, NULL); // Reset color to default
}




void drawLanes(WINDOW* win, const Config& config) {
//drawing end game field
   for (int x = 1; x < config.width - 1; ++x) {
      wcolor_set(win, 1, NULL);
      mvwaddch(win, 1, x, '.');
  }
  wcolor_set(win, 0, NULL);
   //drawing lanes
  for (int i = 0; i < config.num_lanes; ++i) {
      int y = i * config.lane_height + 3;
      for (int x = 1; x < config.width - 1; ++x) {
          mvwaddch(win, y, x, '-');
          mvwaddch(win, y + 1, x, '-');
          mvwaddch(win, y + 2, x, '-');
      }
  }
  wrefresh(win);
}




void drawBorders(WINDOW* win) {
  box(win, 0, 0);
}


void drawGameEntities(WINDOW* win, const Frog& frog, const Obstacle obstacles[], const Config& config) {
  drawLanes(win, config);
  drawFrog(win, frog, config);
  drawObstacles(win, obstacles, config);
}
void displayRealTimeTimer(time_t start_time, int& minutes, int& seconds) {
  time_t current_time;
  time(&current_time);  // Get the current time as a time_t value

  // Calculate the elapsed time in seconds
  int elapsed_seconds = static_cast<int>(difftime(current_time, start_time));

  minutes = elapsed_seconds / 60;
  seconds = elapsed_seconds % 60;

  mvprintw(9, 0, "Time: %d:%02d", minutes, seconds);
  refresh();
}


//                                                          INITIALIZATION


void initializeCars(Car cars[], const Config& config) {

for (int i = 0; i < config.num_cars; ++i) {
    int pos = i * config.lane_height + 3;
    cars[i].y = pos + 1;
    cars[i].x = config.width - 2;  // Off-screen at the start
    cars[i].symbol = config.symbolCar;
    cars[i].move_interval = 3 + rand() % 6;
    cars[i].spawn_time = rand() % MAX_FRAME;  // Random spawn time between 0 and 19 frames
    cars[i].on_field = true;
    cars[i].respawn_time = 0;


    cars[i].last_move_frame = 0;
    cars[i].type = rand() % 2;
    //non-enemy car type
    if (cars[i].type == NEUTRAL) {
        cars[i].type = (rand() % 2 == 0) ? FRIENDLY : NEUTRAL;
    }
    }
}








void reinitializeCar(Car& car, int lane_index, const Config& config) {
  car.x = config.width - 2;
  car.y = (lane_index * config.lane_height + 3) + 1;
  car.symbol = config.symbolCar;
  car.move_interval = 3 + rand() % 6;
  car.spawn_time = rand() % MAX_FRAME;
  car.type = rand() % 2;
  if (car.type == NEUTRAL) {
      car.type = (rand() % 2 == 0) ? FRIENDLY : NEUTRAL;
  }
  car.on_field = true;
  car.last_move_frame = 0;
}




void initializeObstacles(Obstacle obstacles[], Car car[], const Config& config) {

    for (int i = 0; i < config.num_obstacles; ++i) {
      bool validPosition = false;
      obstacles[i].x = 2 + rand() % (config.width - 4);
        //trying to get a proper position for an obstacle
      while (!validPosition) {
          obstacles[i].y = 3 + rand() % (config.height - 4);
          validPosition = true;
          for (int j = 0; j < config.num_cars; j++) {
              //if the obstacle is on the car line or frog initial position it needs to be changed
              if (obstacles[i].y == car[j].y) {
                  validPosition = false;
                  break;
              }
              if ((obstacles[i].x ==config.width / 2)&&(obstacles[i].y==config.height - 2) ) {
                  validPosition = false;
                  break;
              }
          }
      }
      obstacles[i].symbol = config.symbolObstacle;
  }
}
void initializeGameEntities(Frog& frog, Car cars[], Obstacle obstacles[], Timer& timer, Config& config) {
  frog = {config.width / 2, config.height - 2, config.symbolObstacle};
  initializeCars(cars, config);
  initializeObstacles(obstacles, cars, config);
  timer = {100, 0.0f, 0};
}
//                                                               HANDLING CAR MOVEMENT
void changeCarSpeed(Car cars[], const Config& config, int timer_frame_no) {
 int carIndex = rand() % config.num_cars;
 // Change its speed every 100 frames
 if (timer_frame_no % SPEED_INTERVAL == 0) {
     int newSpeed = 2 + rand() % 8;
     cars[carIndex].move_interval = newSpeed;
 }
}


bool isCloseToFrog(Car& car, Frog& frog) {
 //check if the frog is in the front
 if (frog.x < car.x) {


     return (abs(car.y - frog.y) <= 1 && (car.x - frog.x) < 8);
 }
   else {
     return (abs(car.y - frog.y) <= 1 && (frog.x - car.x) < 2);
 }
}




void moveCars(WINDOW* win, Car cars[], Timer& timer, Frog& frog, const Config& config) {
for (int i = 0; i < config.num_cars; ++i) {
   //respawn the car if needed
    if (!cars[i].on_field && timer.frame_no >= cars[i].respawn_time) {
        reinitializeCar(cars[i], i, config);
    }
    if (!cars[i].on_field) {
        continue;
    }


    mvwaddch(win, cars[i].y, cars[i].x, ' ');
    //stop the car if by the close and has sufficient type
    if ((cars[i].type == NEUTRAL ||cars[i].type==FRIENDLY)&& isCloseToFrog(cars[i], frog)) {
        drawCar(win, cars[i],config);
        continue;
    }



//move the cars to the left
    if (timer.frame_no - cars[i].last_move_frame >= cars[i].move_interval) {
        cars[i].x -= 1;
        cars[i].last_move_frame = timer.frame_no;
    }
    drawCar(win, cars[i],config);

//handle cars reaching the end
    if (cars[i].x == 1) {
        if (cars[i].hasFrog) {
            cars[i].hasFrog = false;
            frog.x = cars[i].x;
            frog.y = cars[i].y-1;
            frog.inTheCar=false;
        }
        // Random chance for the car to disappear
        if (rand() % 2 == 0) {
            cars[i].on_field = false;
            cars[i].respawn_time = timer.frame_no + (rand() % 50 + 80);
        } else {
            cars[i].x = config.width - 2;  //reset to the right side
        }
    }
}
}
void collisionF(Car cars[], Frog& frog, bool &collision,int& collisionCounter,const Config& config) {
for (int i = 0; i < config.num_cars; ++i) {
    if(cars[i].type==ENEMY){
    if (cars[i].x == frog.x && cars[i].y == frog.y) {
        collisionCounter++;
        collision = true;
    }
}
}
}


int calculateScore(int moveCounter, int collisionCounter, int minutes, int seconds, int& score) {

   int totalElapsedSeconds = (minutes * 60) + seconds;

   int timePenalty = (totalElapsedSeconds / TIME_INTERVAL) * TIME_PENALTY;

   int movePenalty = 0;
   if (moveCounter > MOVE_CAP) {
       movePenalty = (moveCounter - MOVE_CAP) * MOVE_PENALTY;
   }

   score = BASE_SCORE - movePenalty - (collisionCounter * COLLISION_PENALTY) - timePenalty;


   // Ensure score is non-negative
   return (score > 0) ? score : 0;
}




void waitToQuit(WINDOW *win, bool &gameOver, int moveCounter, int collisionCounter, int minutes, int seconds,const Config& config,
    bool fail=false) {
    int score = 0;
    int finalScore=0;
    if(fail) {
        finalScore=0;
    }
    else{
    finalScore = calculateScore(moveCounter, collisionCounter, minutes, seconds, score);
    }



   mvwprintw(win, (config.height / 2)+1, config.width / 2 - 5, "Total Score: %d", finalScore);
   wrefresh(win);


   // wait for the 'q' to quit
   chtype resumeInput;
   do {
       resumeInput = wgetch(win);
   } while (resumeInput != QUIT);


   gameOver = true;
}


void endGame(WINDOW *win,bool collision,bool victory, bool &gameOver, chtype input, const Config& config,int& moveCounter,
   int& collisionCounter, int& minutes,int& seconds) {
//player reached the end
    if(victory==true) {
     mvwprintw(win, config.height / 2, config.width / 2 - 5, "YOU WON!");
    wrefresh(win);
     waitToQuit(win, gameOver,moveCounter,collisionCounter,minutes,seconds,config);
}
 if (input==QUIT) {
    mvwprintw(win, config.height / 2, config.width / 2 - 5, "GAME OVER!");
    wrefresh(win);
     wrefresh(win);
     waitToQuit(win, gameOver,moveCounter,collisionCounter,minutes,seconds,config,true);


}
    //player got hit by a car
if (collision==true){
    mvwprintw(win, config.height / 2, config.width / 2 - 5, "GAME OVER!");
    wrefresh(win);
   waitToQuit(win, gameOver,moveCounter,collisionCounter,minutes,seconds,config,true);
}
}




bool isObstacleAt(int x, int y, const Obstacle obstacles[],const Config& config, const Car cars[],int& collisionCounter) {
 for (int i = 0; i < config.num_obstacles; ++i) {
     if (obstacles[i].x == x && obstacles[i].y == y) {
         collisionCounter++;
         return true;
     }
 }
   //treat cars as the obstacles too
 for (int i = 0; i < config.num_cars; ++i) {
     if (cars[i].x == x && cars[i].y == y && (cars[i].type == NEUTRAL || cars[i].type == FRIENDLY)) {
         collisionCounter++;
         return true;


     }
 }
 return false;
}






void hopInCar(WINDOW* win, Frog& frog, chtype input, Car cars[], const Config& config,const Obstacle obstacles[],int& collisionCounter) {


     if (input == INTERACT) {
         //hop out
         if (frog.inTheCar) {
             for (int i = 0; i < config.num_cars; ++i) {
                 if (cars[i].hasFrog && cars[i].type == FRIENDLY) {
                     int targetX = cars[i].x;
                     int targetY = cars[i].y - 1; // The position where the frog will hop out

                     // use the isObstacleAt function to check if there is an obstacle
                     if (isObstacleAt(targetX, targetY, obstacles, config, cars, collisionCounter)) {
                         mvwprintw(win, config.height / 2, config.width / 2 - 10, "Can't exit! Obstacle ahead.");
                         wrefresh(win);
                         return;
                     }

                     cars[i].hasFrog = false;
                     frog.inTheCar = false;
                     frog.x = cars[i].x;
                     frog.y = cars[i].y-1;
                     clearFrog(win, frog);
                     break;
                 }
             }




         }
         //hop in
         else {
             for (int i = 0; i < config.num_cars; ++i) {
                 if (cars[i].type == FRIENDLY && abs(cars[i].x - frog.x) <= 2 && abs(cars[i].y - frog.y) <= 1) {
                     //frog is set to start point and is unable to move
                     frog.x = config.width/2;
                     frog.y = config.height-2;
                     cars[i].hasFrog = true;
                     frog.inTheCar = true;
                     clearFrog(win, frog);
                     drawCar(win, cars[i],config);
                     break;// Stop checking other cars
                 }
             }
         }
     }
}








void frogMove(WINDOW* win, Frog& frog, chtype input, bool &victory, const Obstacle obstacles[], const Config& config,const Car cars[], int& moveCounter,int& collisionCounter) {
  clock_t current_time = clock();
  double elapsed_time = double(current_time - frog.last_move_time) / CLOCKS_PER_SEC;




  if (elapsed_time < 0.01) {  // check if at least 1 second has passed
      return;
  }
  clearFrog(win, frog);


  frog.last_move_time = current_time;
 if(!frog.inTheCar) {
     int new_x = frog.x;
     int new_y = frog.y;
     if (input == GO_UP && frog.y > 1) {
         new_y-=2;




         if (new_y <= 1) {
             clearFrog(win,frog);
             victory = true;
         }
     } else if (input == GO_LEFT && frog.x > 2) {
         new_x-=2;


     } else if (input == GO_DOWN && frog.y < config.height - 2) {
         new_y+=2;

     } else if (input == GO_RIGHT && frog.x < config.width - 3) {
         new_x+=2;
     }

     // check if the new position is free of obstacles
     if (new_x != frog.x || new_y != frog.y) {
         if (!isObstacleAt(new_x, new_y, obstacles, config, cars,collisionCounter)) {
             frog.x = new_x;
             frog.y = new_y;
             ++moveCounter;
         }
     }
 }

 drawFrog(win, frog,config);
}
void loadGameState(WINDOW* win, const char* filename, Frog& frog, Car cars[], int& moveCounter,int& collisionCounter) {
   FILE* file = fopen(filename, "r");
   char key[30];
   char value[20];
   clearFrog(win, frog);

   while (fscanf(file, "%[^=]=%s\n", key, value) == 2) {
       if (strcmp(key, "FROG_X") == 0) {
           frog.x = atoi(value);
       } else if (strcmp(key, "FROG_Y") == 0) {
           frog.y = atoi(value);
       } else if (strcmp(key, "FROG_IN_THE_CAR") == 0) {
           frog.inTheCar = atoi(value) != 0;
       } else if (strcmp(key, "MOVE_COUNTER") == 0) {
           moveCounter = atoi(value);
       }
       else if (strcmp(key, "COLLISION_COUNTER") == 0) {
           collisionCounter = atoi(value);
       }


       else if (strncmp(key, "CAR", 3) == 0) {
           int i = atoi(key + 4);
           if (strstr(key, "_X")) {
               cars[i].x = atoi(value);
           } else if (strstr(key, "_Y")) {
               cars[i].y = atoi(value);
           } else if (strstr(key, "_ON_FIELD")) {
               cars[i].on_field = atoi(value) != 0;
           } else if (strstr(key, "_TYPE")) {
               cars[i].type = atoi(value);
           } else if (strstr(key, "_MOVE_INTERVAL")) {
               cars[i].move_interval = atoi(value);
           } else if (strstr(key, "_HAS_FROG")) {
               cars[i].hasFrog = atoi(value) != 0;
           }
       }
   }


   fclose(file);
}


void displayCollisionCount(int collisionCount) {
   mvprintw(14, 1, "Collisions:");
   mvprintw(15, 1, "%d", collisionCount);
   refresh();
}


void pauseGame(WINDOW *win, const Config& config, Car cars[]) {
 for (int i = 0; i <config.num_cars; ++i) {
     drawCar(win, cars[i],config);
 }
  mvwprintw(win, config.height / 2, config.width / 2 - 5, "GAME PAUSED");
  mvprintw(6, 1, "Paused");
  refresh();
  wrefresh(win);

  while (true) {
      int ch = getch();
      if (ch == ESC) {
          mvwprintw(win, config.height / 2, config.width / 2 - 5, "           "); // Clear text
          wrefresh(win);
          mvprintw(6, 1, "Active");
          refresh();
          break;
      }
  }
}






void takeTheInput(chtype input,WINDOW* win, Frog& frog, Car cars[], Config& config, Timer& timer, bool& victory, bool& collision, const Obstacle obstacles[],int& moveCounter,int minute, int second,time_t start_time,int& collisionCounter) {


  if (input != ERR) {
      if (input == PRINT) {
          saveGame("/Users/miawwww/BasComp2425/Project_1_jumping_frog/game_save.txt", frog, cars, config,moveCounter,collisionCounter);
      }
      else if (input == LOAD) {
          loadGameState(win, "/Users/miawwww/BasComp2425/Project_1_jumping_frog/game_save.txt", frog, cars,moveCounter,collisionCounter);
          wrefresh(win);
          // Reinitialize timer based on loaded state
          drawGameEntities(win, frog, obstacles, config);
      }
      else if (input == ESC) {
          pauseGame(win,config,cars);
      }
      frogMove(win, frog, input, victory, obstacles, config,cars,moveCounter,collisionCounter);
      hopInCar(win, frog, input, cars, config,obstacles,collisionCounter);
      collisionF(cars, frog, collision,collisionCounter,config);
  }
}
void runGameLoop(WINDOW* win, Frog& frog, Car cars[], Obstacle obstacles[], Config& config, Timer& timer, bool& gameOver, bool& victory, bool& collision, time_t start_time, int& moveCounter, int& minute, int& second,int& collisionCounter,int& minutes,int& seconds) {
  while (!gameOver) {
      drawGameEntities(win, frog, obstacles, config);


      chtype input = wgetch(win);


      takeTheInput(input, win, frog, cars, config, timer, victory, collision, obstacles,moveCounter,minute,second,start_time,collisionCounter);


      moveCars(win, cars, timer, frog, config);
      changeCarSpeed(cars, config, timer.frame_no);


      collisionF(cars, frog, collision,collisionCounter,config);


      displayMoveCounter(moveCounter);
      displayRealTimeTimer(start_time,minutes, seconds);
      displayCollisionCount(collisionCounter);
      endGame(win, collision, victory, gameOver, input, config,moveCounter,collisionCounter,minutes,seconds);




      timer.frame_no++;
      timer.pass_time += timer.frame_time / 1000.0f;


      // delay for 10 milliseconds
      usleep(10000);


      drawBorders(win);
      wrefresh(win);
  }
}




int frogGame(WINDOW* win) {
 bool gameOver = false;
 bool victory = false;
 bool collision = false;
  int moveCounter=0;
   int seconds=0;
   int minutes=0;
   int collisionCounter=0;
 Config config = readConfig("/Users/miawwww/BasComp2425/Project_1_jumping_frog/game_config.txt");
  Obstacle obstacles[config.num_obstacles];
  Frog frog;
  Car cars[config.num_cars];
  Timer timer;

  initializeGameEntities(frog, cars, obstacles, timer, config);
  time_t start_time = time(NULL);
  drawGameEntities(win, frog, obstacles, config);
 nodelay(win, TRUE);  // non-blocking input (no wait for key press)
  runGameLoop(win, frog, cars, obstacles, config, timer, gameOver, victory, collision, start_time,moveCounter,minutes,seconds,collisionCounter,minutes,seconds);
 return gameOver;
}



