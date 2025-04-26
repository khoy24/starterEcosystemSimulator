#include "ecosim.h"
#include <string.h>
#include <unistd.h>
#include <ncurses.h>
#include <vector>
#include <iostream>
#include <time.h>

int numrabbits = 0;
Rabbit rabbits[HEIGHT][WIDTH];

void spawnRabbits(){

    while (numrabbits < 10){
        //color
        int randcolor = rand() % 2;
        if (randcolor == 1){
            randcolor = 4; //assign it yellow if not white
        }

        //gender
        int randgen = rand() % 2;
        char randgender;
        if (randgen == 0){
            randgender = 'M';
        } else {
            randgender = 'F';
        }

        //speed (should all spawn with 1 for now)
        //sightradius (should all spawn with 1 for now)
        //hunger (spawn with full hunger)
        //thirst (spawn with full thirst)

        //og age should be from 20->40 (in their primes)
        //average lifespan of rabbit is 7-10 years. lets say 70-100 in this case. Babies are 0->10
        int randage = 20 + rand() % 21;
        //baby rabbits = r, adult rabbits = R

        //pregnant spawns as not.
    
        int x = rand() % WIDTH;
        int y = rand() % HEIGHT;
        if (terrain[y][x].symbol == '^' || terrain[y][x].symbol == '#'){
            //color, gender, speed, sightradius, hunger, thirst, age, pregnancy;
            rabbits[y][x] = Rabbit(randcolor, randgender, 1, 1, 'R', 100, 100, randage, 0);
            numrabbits ++;
        }
    }
}