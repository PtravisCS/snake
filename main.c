/*
 * Title        : Snake
 * Purpose      : A C based clone of the game Snake originally hailing from the late 70's.
 *                The objective of the game is to control a line using the arrow keys, having
 *                the line pick up objects which cause it to grow longer. The player loses when
 *                they hit the edge of the play area or when they hit themselves.
 *
 * Author       : Paul Travis
 * Date         : 11/5/2020
 *
 * Dependancies : This program reuires the Ncurses Library
 */

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>

/*
 * Name     : Coords
 * Purpose  : Used to represent a set of coordinates on an x,y plane
 * Author   : Paul Travis
 * Dep.     : None
 */
struct Coords {

  int xcoord; //Used to represent an x coordinate where 0 represents the begenning of a line
  int ycoord; //Used to represent a y coordinate where 0 represents the top line

};

/*
 * Name     : Snake
 * Purpose  : Used to represent the snake object in the game.
 *            This includes the snakes x and y velocity, it's current size, and an array of it's coordinates.
 *
 * Author   : Paul Travis
 * Dep.     : Coords
 */
struct Snake {

  int xvel; //The snakes x velocity. Represented as a 1 or a -1, where 1 is right and -1 is left.
  int yvel; //The snakes y velocity. Represented as a 1 or a -1, where 1 is down and -1 is up.

  int size; //The size (length) of the snake. 

  struct Coords coords[200]; //An array of coordinates representing the snakes current [0] and past coordinates [1 - x]

};


/*
 * Name     : end
 * Purpose  : Used to gracefully end the game
 * Author   : Paul Travis
 */
void end(char *message) {

  mvprintw(LINES / 2, COLS / 3, message);

  timeout(-1); //Reset the wait timeout to the default -1 (wait indefinitly)

  getch(); //Wait for a keypress and then throw away the recieved char

  endwin(); //End the ncurses window session

  exit(0); //Exit the program

}

/*
 * Name     : randInRange
 * Purpose  : Used to return a random integer within a given range
 * Author   : Paul Travis
 */
int randInRange(int low, int high) {

  srand(time(0));
  return rand() % (high + 1 - low) + low;

}

int main(int argc, char *argv[]) {

  struct Snake snake; //Instantiate an instance of the snake 
  struct Coords pellet; //Create a coord pair to represent the pellets the snake eats
  snake.xvel = 0; //Set the x velocity of the snake to 0.
  snake.yvel = 0; //Set the y velocity of the snake to 0.
  snake.size = 1; //Set the size of the snake to 0 (size of the snake is 0 based as opposed to 1 based).
  pellet.ycoord = randInRange(3, (LINES - 2));
  pellet.xcoord = randInRange(3, (COLS - 2));

  for (int i = 0; i < 20; i++) { //Set all previous coords to -1 which the game treats as an empty coordinate set.

    snake.coords[i].xcoord = -1; 
    snake.coords[i].ycoord = -1;

  }

  snake.coords[0].xcoord = 4; //Set the initial x coord for the snake to 4 
  snake.coords[0].ycoord = 4; //Set the initial y coord for the snake to 4

  initscr(); //Initialize the ncurses screen instance
  clear();  //Clear the screen (I really wish a library wasn't necessary for this behaviour
  noecho(); //Disable user input echoing
  cbreak(); //Disable line buffering and erase/kill char processing allowing chars typed by user to be available to program immediately
  timeout(150); //Set a wait timeout, this enables the game to continue without waiting (long) for each getch() operation to complete
  curs_set(0); //Hide the terminal cursor
  keypad(stdscr, TRUE); //Enables the keypad permitting use of KEY_LEFT etc.

  printw("Press 'q' to quit.");
  refresh(); //Refresh screen to display the exit message
  
  for (int ch = getch(); ch != 'q'; ch = getch()) { //Loop until the user presses q

    if (ch == KEY_UP) { //If the up key is pressed
  
      snake.xvel = 0;
      snake.yvel = -1;

    } else if (ch == KEY_DOWN) { //If the down key is pressed

      snake.xvel = 0;
      snake.yvel = 1;

    } else if (ch == KEY_LEFT) { //If the left key is pressed

      snake.xvel = -1;
      snake.yvel = 0;

    } else if (ch == KEY_RIGHT) { //If the right key is pressed

      snake.xvel = 1;
      snake.yvel = 0;

    }

    clear(); //Clear the screen.

    mvprintw(0, 0, "Press 'q' to quit."); //Print the message indicating how to quit the game.

    for (int i = 2; i != (COLS - 1); i++) { //Print the top and bottom borders of the map.

      mvprintw(2, i, "X");
      mvprintw(LINES - 2, i, "X");

    }

    for (int i = 2; i != (LINES - 1); i++) { //Print the left and right borders of the map.

      mvprintw(i, 2, "X");
      mvprintw(i, COLS - 2, "X");

    }


    for (int i = snake.size; i > 0; i--) { //Re-arrange the snakes coords to match it's new position.

      snake.coords[i].ycoord = snake.coords[i - 1].ycoord;
      snake.coords[i].xcoord = snake.coords[i - 1].xcoord;

    }

    snake.coords[0].xcoord += snake.xvel; //Move the snake in the x direction associated with it's x velocity.
    snake.coords[0].ycoord += snake.yvel; //Move the snake in the y direction associated with it's y velocity.

    for (int i = 1; i < snake.size; i++) { //Check for snake collision

      if (snake.coords[i].ycoord == snake.coords[0].ycoord && snake.coords[i].xcoord == snake.coords[0].xcoord) { //If the head of the snake collides with the tail

        if (snake.xvel != 0 || snake.yvel != 0) { //And if the snake is moving

          end("Game Over! Press any key to continue"); //Game Over    

        }

      }


    }

    if (snake.coords[0].ycoord < 3 || snake.coords[0].xcoord < 3) { //Check if snake goes off left or top of map

      end("Game Over! Press any key to continue"); //Game Over    

    }

    if (snake.coords[0].ycoord > (LINES - 3) || snake.coords[0].xcoord > (COLS -  3)) { //Check if snake goes off right or bottom of map

      end("Game Over! Press any key to continue"); //Game Over    

    }

    if (snake.coords[0].ycoord == pellet.ycoord && snake.coords[0].xcoord == pellet.xcoord) {

      snake.size += 1;
      pellet.ycoord = randInRange(3, (LINES - 3));
      pellet.xcoord = randInRange(3, (COLS - 3));
      printf("\a");

    }

    for (int i = 0; i < snake.size; i++) { //Print the snake to the screen

      mvprintw(snake.coords[i].ycoord, snake.coords[i].xcoord, "*");
      //mvprintw(i + 2,1,"%i: %i,%i", i, snake.coords[i].ycoord, snake.coords[i].xcoord); //uncomment this line to see the snake's coordinate array.

    }

    mvprintw(pellet.ycoord, pellet.xcoord, "@");
    mvprintw(1,1,"x: %i, y: %i", pellet.xcoord, pellet.ycoord);

    refresh();

  }

  end("Thanks for playing");
}
