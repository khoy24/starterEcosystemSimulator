#include "ecosim.h"
#include <string.h>
#include <unistd.h>
#include <ncurses.h>
#include <vector>
#include <iostream>
#include <time.h>
#include <cmath>
#include <algorithm>  // Include this header for std::max_element

void print_terrain();
void plotpopulation();
std::vector<int> points;  // population of rabbits as points on y-axis
std::vector<int> days;    

int main(int argc, char *argv[]) {
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

    int daysleft = 79;
    int day = 0;
    while (daysleft > 0) {
        moveRabbits();
        print_terrain();

        points.push_back(numrabbits);  
        numrabbits+= 10; 
        days.push_back(day);  

        refresh();  
        usleep(250000);
        day++;
        daysleft--;
    }

    plotpopulation(); 

    getch();
    endwin();

    return 0;
}

void print_terrain() {
    // Print the terrain
    int i, j;
    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            // Print rabbits first
            if (rabbits[i][j].symbol != ' ') {
                attron(COLOR_PAIR(rabbits[i][j].color));
                mvaddch(1 + i, 20 + j, rabbits[i][j].symbol);
                attroff(COLOR_PAIR(rabbits[i][j].color));
            } else {
                attron(COLOR_PAIR(terrain[i][j].color));
                mvaddch(1 + i, 20 + j, terrain[i][j].symbol);
                attroff(COLOR_PAIR(terrain[i][j].color));
            }
        }
        mvaddch(1 + i, 20 + j, '\n');
    }
}

// plots the graph of the rabbit population, scaled to terminal size
void plotpopulation() {

    clear();

    int maxval = *std::max_element(points.begin(), points.end()); //gets max numrabbits

    //assigns y-axis marks with the necessary distance in between
    double yaxisincrementprev = maxval / 20.0;
    int yaxisincrement = ceil(yaxisincrementprev);
    // vector consisting of all y-axis mark ticks. should be checked for which value is closest for each print.
    std::vector<int> yaxismarks;
    int starter = 0;
    while (starter < maxval){
        yaxismarks.push_back(starter);
        starter += yaxisincrement;
    }

    // scale to typical terminal size (or in this case our terrains), 80 x 20
    for (size_t d = 0; d < points.size(); d++) { 

        int currentpoint = points[d];
        int closesttick = 0;
        //the row corresponding to the closest tick to the population point
        int tickrow = 0;

        // find the closest tick mark
        for (size_t i = 0; i < yaxismarks.size(); i++) {
            int tick = yaxismarks[i];
            if (abs(currentpoint - tick) < abs(currentpoint - closesttick)) {
                closesttick = tick;
                tickrow = i; 
            }
        }

        // plots the graph points as *
        mvaddch(HEIGHT - tickrow, 20 + d, '*'); 
    }

    //add title
    mvprintw(0, 35, "Rabbit Population graphed over time");
    //add y-axis labels
    int count = 0;
    for (int i : yaxismarks){
        mvprintw(HEIGHT - count, 5, "%d", i);
        count ++;
    }

    refresh();  
}



