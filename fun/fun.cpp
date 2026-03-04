#include "fun.h"
#include <ncurses.h>
#include <unistd.h>
#include <ctime>
#include <cmath>


 int maxY, maxX;
 getmaxyx(stdscr, maxY, maxX);

 // Create the game window
 box(window, 0, 0);
 wrefresh(window);
 mvprintw(1, 0, "Name");
 mvprintw(2, 1, " Surname");
 mvprintw(3, 1, "student id");


 return window;
}




}


      wrefresh(win);
}




void clearFrog(WINDOW* win, const Frog& frog) {
  mvwprintw(win, frog.y, frog.x, "  ");
  wrefresh(win);
}



      wcolor_set(win, 4, NULL);
      mvwaddch(win, obstacles[i].y, obstacles[i].x, obstacles[i].symbol);
      wcolor_set(win, 0, NULL);
  }
  wrefresh(win);
}





     }
 }
 wrefresh(win);
 wcolor_set(win, 0, NULL); // Reset color to default

}





    cars[i].y = pos + 1;
    cars[i].move_interval = 3 + rand() % 6;
    cars[i].respawn_time = 0;


    cars[i].last_move_frame = 0;
    cars[i].type = rand() % 2;
    }

  }
}



 // Change its speed every 100 frames
     cars[carIndex].move_interval = newSpeed;
 }
}

 }
   else {
 }
}


    if (!cars[i].on_field && timer.frame_no >= cars[i].respawn_time) {
    }
    if (!cars[i].on_field) {
        continue;
    }


    mvwaddch(win, cars[i].y, cars[i].x, ' ');
        continue;
    }



    if (timer.frame_no - cars[i].last_move_frame >= cars[i].move_interval) {
        cars[i].last_move_frame = timer.frame_no;
    }

    if (cars[i].x == 1) {
        // Random chance for the car to disappear
            cars[i].on_field = false;
        } else {
        }
    }
}
}
    if (cars[i].x == frog.x && cars[i].y == frog.y) {
        collision = true;
    }

   }


}




   wrefresh(win);
   gameOver = true;
}


    wrefresh(win);
}


if (collision==true){
    wrefresh(win);
}




     if (obstacles[i].x == x && obstacles[i].y == y) {
                 }
             }

         }
                 }

  }


     int new_x = frog.x;
     int new_y = frog.y;

             victory = true;
         }
     }

             frog.x = new_x;
             frog.y = new_y;
   }

}



  refresh();

          refresh();
}





      }
      }
      }
  }
}
  while (!gameOver) {














      timer.frame_no++;
      timer.pass_time += timer.frame_time / 1000.0f;


      drawBorders(win);
      wrefresh(win);
  }

 return gameOver;
}
