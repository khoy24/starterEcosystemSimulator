#include "ecosim.h"
#include <string.h>
#include <unistd.h>
#include <ncurses.h>
#include <vector>
#include <iostream>
#include <time.h>
#include <cmath>

int numrabbits = 0;
Rabbit* rabbits[HEIGHT][WIDTH];
std::vector<Rabbit*> rabbitlist;
void clearrabbits();
std::vector<int> lookforfood(Rabbit* r);
std::vector<int> lookforwater(Rabbit* r);

void spawnRabbits(){

    while (numrabbits < 30){
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
        int randage = 3 + rand() % 5;
        //baby rabbits = r, adult rabbits = R

        //pregnant spawns as not.
    
        int x = rand() % WIDTH;
        int y = rand() % HEIGHT;
        if ((terrain[y][x].symbol == '^' || terrain[y][x].symbol == '#') && (rabbits[y][x]->symbol != 'R' && rabbits[y][x]->symbol!='r')){
            //color, gender, speed, sightradius, symbol, hunger, thirst, age, pregnancy, dob, y, x;
            delete rabbits[y][x];
            rabbits[y][x] = new Rabbit(randcolor, randgender, 1.0, 1, 'R', 100, 100, randage, 0, 0, y, x);
            rabbitlist.push_back(rabbits[y][x]);
            numrabbits ++;
        }
    }
}

void moveRabbits(){

    // clearrabbits();

    for (Rabbit* r : rabbitlist){
        double speed = r->speed; // how many spaces the rabbit will move this round
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

            int targetx = -1;
            int targety = -1;
            int hungry = 0;
            //if hungry
            if (r->hunger < 70 && r->thirst > r->hunger){
                std::vector<int> targets = lookforfood(r);
                targetx = targets[0];
                targety = targets[1];
                hungry = 1;
            // if thirsty
            } else if (r->thirst < 70){
                std::vector<int> targets = lookforwater(r);
                targetx = targets[0];
                targety = targets[1];
            }
            //the rabbit is searching for food, need to account for speed
            if (hungry==1){
                int newx = r->x;
                int newy = r->y;
                // check x direction to get closer (for now all rabbits have a speed of 1, but later we will have to check for 
                // higher speeds checking moving less than what they are capable of if there is an obstacle)
                if (r->x > targetx && (int)(r->x - speed) >= targetx && (int)(r->x - speed) >= 0 && (int)(r->x - speed) < WIDTH){
                    if (terrain[r->y][(int)(r->x - speed)].symbol!='~' && rabbits[r->y][(int)(r->x - speed)]->symbol == ' '){ //checks if the x left direction is open
                        newx = (int)(r->x-speed);
                    }
                } else if (r->x < targetx && (int)(r->x + speed) <= targetx && (int)(r->x + speed)>=0 && (int)(r->x + speed)< WIDTH){
                    if (terrain[r->y][(int)(r->x + speed)].symbol!='~' && rabbits[r->y][(int)(r->x + speed)]->symbol == ' '){ //checks if the x right direction is open
                        newx = (int)(r->x+speed);
                    }
                }
                //now check y direction to get closer
                if (r->y > targety && (int)(r->y - speed) >= targety && (int)(r->y - speed)>= 0 && (int)(r->y - speed) < HEIGHT){
                    if (terrain[(int)(r->y - speed)][newx].symbol!='~' && rabbits[(int)(r->y - speed)][newx]->symbol == ' '){ //checks if the y up direction is open
                        newy = (int)(r->y-speed);
                    }
                } else if (r->y < targety && (int)(r->y + speed) <= targety && (int)(r->y + speed)>=0 && (int)(r->y + speed)<HEIGHT){
                    if (terrain[(int)(r->y + speed)][newx].symbol!='~' && rabbits[(int)(r->y + speed)][newx]->symbol == ' '){ //checks if the y down direction is open
                        newy = (int)(r->y+speed);
                    }
                }
                //move the empty space to the rabbits about to be previous space and update the pointers x and y's.
                rabbits[r->y][r->x] = rabbits[newy][newx];
                rabbits[r->y][r->x]->y = r->y;
                rabbits[r->y][r->x]->x = r->x;

                rabbits[newy][newx]= r;

                r->x = newx;
                r->y = newy;
                moved = 1;

            }

            //determine if completely surrounded by rabbits so it can stand still
            int nowheretogo = 1;
            int i,j;
            for (i=r->y-1; i < r->y+2 && i <HEIGHT;i++){
                for (j=r->x-1; j < r->x+2 && j < WIDTH; j++){
                    if (i >= 0 && j >= 0 && rabbits[i][j]->symbol!='r' && rabbits[i][j]->symbol!='R' && terrain[i][j].symbol!='~'){
                        nowheretogo = 0;
                    }
                }
            }
            if (nowheretogo==0){
                moved = 1; //need to move this out so rabbits can stand still.
            }


            int newxcheck = r->x + rand() % (int)(speed + 1);
            int negchance = rand() % 2;
            if (negchance == 1){
                newxcheck = r->x - newxcheck + r->x;
            }
            int newycheck = r->y + rand() % (int)(speed + 1);
            int negchance2 = rand() % 2;
            if (negchance2 == 1){
                newycheck = r->y - newycheck + r->y;
            }
            if (newycheck >= 0 && newycheck < HEIGHT && newxcheck >= 0 && newxcheck < WIDTH && terrain[newycheck][newxcheck].symbol != '~' && rabbits[newycheck][newxcheck]->symbol!='R' && rabbits[newycheck][newxcheck]->symbol!='r'){
                
                //move the empty space to the rabbits about to be previous space and update the pointers x and y's.
                rabbits[r->y][r->x] = rabbits[newycheck][newxcheck];
                rabbits[r->y][r->x]->y = r->y;
                rabbits[r->y][r->x]->x = r->x;

                rabbits[newycheck][newxcheck]= r;

                r->x = newxcheck;
                r->y = newycheck;
                moved = 1;
            }
            moved = 1;

        } 
        //if you remove the section above after hunger a rabbit won't leave its bush (until its thirsty later) otherwise this has random movement


    }
}

void clearrabbits(){
    int i,j;
    for (i = 0; i < HEIGHT; i++){
        for (j = 0; j < WIDTH; j++){
            rabbits[i][j]->symbol = ' ';
        }
    }
}

std::vector<int> lookforfood(Rabbit* r){
    int i,j;
    int closestfoodx, closestfoody = -1;
    int closestfooddistance = 1000;

    for (i=r->y - r->sightradius; i <= r->y + r->sightradius && i >=0 && i < HEIGHT; i++){
        for (j=r->x-r->sightradius; j <= r->x + r->sightradius && j>=0 && j <WIDTH; j++){
            if (terrain[i][j].symbol=='#'){
                //need to take into account speed here (how far they can move) because if they can't move there directly,
                //we need to remember where they can go cuz if theres food closer they'd want to go to close one but would pop up after
                //some in the loop potentially. 
                //Have a closest food source terrain target??
                // if (abs(r->y - i) <= abs(r->y - closestfoody) || abs(r->x-j) <= abs(r->x - closestfoodx)){ // I think this might be a problem. what if one variable is way further?
                                                                              // actually its a square so probably not?
                                                                                 // I think I should implement x^2 + y^2 formula
                if ((i - r->y)*(i - r->y) + (j - r->x)*(j - r->x) < closestfooddistance){
                    closestfoodx = j;
                    closestfoody = i;
                    closestfooddistance = (i - r->y)*(i - r->y) + (j - r->x)*(j - r->x);
                }
            }
        }
    }
    return {closestfoodx, closestfoody};
}

std::vector<int> lookforwater(Rabbit* r){
    int i,j;
    int closestwaterx, closestwatery = -1;
    int closestwaterdistance = 1000;

    for (i=r->y - r->sightradius; i <= r->y + r->sightradius && i >=0 && i < HEIGHT; i++){
        for (j=r->x-r->sightradius; j <= r->x + r->sightradius && j>=0 && j <WIDTH; j++){
            if (terrain[i][j].symbol=='~'){
                if ((i - r->y)*(i - r->y) + (j - r->x)*(j - r->x) < closestwaterdistance){
                    closestwaterx = j;
                    closestwatery = i;
                    closestwaterdistance = (i - r->y)*(i - r->y) + (j - r->x)*(j - r->x);
                }
            }
        }
    }
    return {closestwaterx, closestwatery};
}

