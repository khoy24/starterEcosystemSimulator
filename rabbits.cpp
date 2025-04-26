#include "ecosim.h"
#include <string.h>
#include <unistd.h>
#include <ncurses.h>
#include <vector>
#include <iostream>
#include <time.h>
#include <cmath>

int numrabbits = 0;
Rabbit rabbits[HEIGHT][WIDTH];
std::vector<Rabbit> rabbitlist;
void clearrabbits();

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
        if ((terrain[y][x].symbol == '^' || terrain[y][x].symbol == '#') && (rabbits[y][x].symbol != 'R' || rabbits[y][x].symbol=='r')){
            //color, gender, speed, sightradius, symbol, hunger, thirst, age, pregnancy, y, x;
            rabbits[y][x] = Rabbit(randcolor, randgender, 1.0, 1, 'R', 100, 100, randage, 0, y, x);
            rabbitlist.push_back(rabbits[y][x]);
            numrabbits ++;
        }
    }
}

void moveRabbits(){

    clearrabbits();

    for (Rabbit r : rabbitlist){
        double speed = r.speed; // how many spaces the rabbit will move this round
        double largestint = ceil(speed);
        if (largestint - speed > 0){
            //then percentage chance of moving to the largest int = 
            int percentchance = 100 * (1-(largestint-speed));
            int randhund = rand() % 101;

            if (randhund < percentchance){
                //then we can set the speed to the one above
                speed = largestint;
            } else {
                speed = floor(speed);
            }
        } 

        //move rabbit number of spaces that it's speed is, random direction.
        int moved = 0;
        while (moved == 0){
            int newxcheck = r.x + rand() % (int)(speed + 1);
            int negchance = rand() % 2;
            if (negchance == 1){
                newxcheck = r.x - newxcheck + r.x;
            }
            int newycheck = r.y + rand() % (int)(speed + 1);
            int negchance2 = rand() % 2;
            if (negchance2 == 1){
                newycheck = r.y - newycheck + r.y;
            }
            if (newycheck >= 0 && newycheck < HEIGHT && newxcheck >= 0 && newxcheck < WIDTH && terrain[newycheck][newxcheck].symbol != '~' && rabbits[newycheck][newxcheck].symbol!='R' && rabbits[newycheck][newxcheck].symbol!='r'){
                
                rabbits[newycheck][newxcheck]= r;

                r.x = newxcheck;
                r.y = newycheck;
                moved = 1;
            }
        }
    }
}

void clearrabbits(){
    int i,j;
    for (i = 0; i < HEIGHT; i++){
        for (j = 0; j < WIDTH; j++){
            rabbits[i][j].symbol = ' ';
        }
    }
}