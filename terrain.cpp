#include "ecosim.h"
#include <string.h>
#include <unistd.h>
#include <ncurses.h>
#include <vector>
#include <iostream>
#include <time.h>

land terrain[HEIGHT][WIDTH];
// Generates terrain for the environment. Consists of grass, a lake, and bushes (food source)
void generate_terrain(){

    // Originally assign it all grass ^^^^...
    int i,j;
    for (i=0; i < HEIGHT; i++){
        for (j=0; j < WIDTH; j++){
            terrain[i][j].symbol = '^';
            terrain[i][j].color = 3;
        }
    }

    // water

    int watercount = 0; //keeps track of number of water tiles placed

    //makes the terrain covered in at least 1/5s water of small lakes
    while (watercount < (HEIGHT * WIDTH / 5)){
        //choose a random starting point
        int x = rand() % WIDTH;
        int y = rand() % HEIGHT;
        int lakewidth = 3 + rand() % 11; //random lake width between 3 to 10
        int lakeheight = 2 + rand() % 6; //random lake height between 2 to 7
        //make lil lakes 
        for (i = y; i < HEIGHT && i < y + lakeheight; i++){
            for (j = x; j < WIDTH && j < x + lakewidth; j++){
                terrain[i][j].symbol = '~';
                terrain[i][j].color = 5;
                watercount ++ ;
            }
        }
    }

    // food

    // cover the terrain with bushes of food. 
    int bushcount = 0;
    while (bushcount < (HEIGHT * WIDTH / 8)){
        //choose a random starting point
        int x = rand() % WIDTH;
        int y = rand() % HEIGHT;
        //make bushess
        if (terrain[y][x].symbol != '~'){
            terrain[y][x].symbol = '#';
            terrain[y][x].color = 6; //magenta (berry color)
            bushcount ++ ;
        }

    }

}