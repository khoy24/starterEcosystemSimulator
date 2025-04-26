#include "ecosim.h"
#include <string.h>
#include <unistd.h>
#include <ncurses.h>
#include <vector>
#include <iostream>
#include <time.h>

void print_terrain();

int main(int argc, char *argv[]){

    // pass in a seed using system time so randomly generated numbers aren't the same every time
    srand(time(NULL));
    initscr();
 
    raw();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    // initializes color for curses
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);   
    init_pair(2, COLOR_RED, COLOR_BLACK);     
    init_pair(3, COLOR_GREEN, COLOR_BLACK);   
    init_pair(4, COLOR_YELLOW, COLOR_BLACK);  
    init_pair(5, COLOR_BLUE, COLOR_BLACK);    
    init_pair(6, COLOR_MAGENTA, COLOR_BLACK); 
    init_pair(7, COLOR_CYAN, COLOR_BLACK);  

    generate_terrain();
    spawnRabbits();

    // usleep(250000);
    int alive = 10;
    while (alive > 0){
        //move rabbits
        moveRabbits();
        print_terrain();
        refresh();
        usleep(250000);
        alive --;
    }

    refresh();
    //need this so that it waits for a key press to close the window. 
    getch();
    endwin();
    return 1;

}


void print_terrain(){
    //print the terrain
    int i,j;
    for (i=0; i < HEIGHT; i++){
        for (j=0; j < WIDTH; j++){
            //print rabbits first
            if (rabbits[i][j].symbol!=' '){
                attron(COLOR_PAIR(rabbits[i][j].color));
                mvaddch(1 + i, 20 + j, rabbits[i][j].symbol);
                attroff(COLOR_PAIR(rabbits[i][j].color));
            } else {
                attron(COLOR_PAIR(terrain[i][j].color));
                mvaddch(1 + i, 20 + j, terrain[i][j].symbol);
                attroff(COLOR_PAIR(terrain[i][j].color));
            }

            // std::cout << terrain[i][j].symbol;
        }
        // std::cout << std::endl;
        mvaddch(1 + i, 20 + j, '\n');
    }
}
