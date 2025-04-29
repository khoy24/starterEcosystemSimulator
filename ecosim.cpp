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
void plotcolor();
void agerabbits(int day);
void hungerandthirst();
void reproduce();
void rabbitsborn(int day);
std::vector<int> points;  // population of rabbits as points on y-axis
std::vector<int> days;    
std::vector<Rabbit*> newborns;
std::vector<int> yellowrabbits;
std::vector<int> whiterabbits;
std::vector<int> malerabbits;
std::vector<int> femalerabbits;
int numfemales = 0;
int nummales = 0;
int numyellow = 0;
int numwhite = 0;

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
        //check for pregnant rabbits
        rabbitsborn(day);

        moveRabbits();
        print_terrain();

        points.push_back(numrabbits);  
        days.push_back(day);  
        yellowrabbits.push_back(numyellow);
        whiterabbits.push_back(numwhite);

        //check hunger and thirst levels, also have rabbits eat and drink here. 
        hungerandthirst();
        //check if rabbits can breed
        reproduce();

        refresh();  
        usleep(250000);
        day++;
        //check if we should age them up a year
        agerabbits(day);
        daysleft--;
    }

    //can pick what graphs to display
    clear();
    mvprintw(10,20, "View graphs. p : population, c: color, g : gender, s : speed, Q : quit program");
    int32_t key;
    int inprogram = 1;
    while (inprogram==1){
        key = getch();
        switch (key) {
            case 'p':
                //attempt to move pc one cell upper left
                plotpopulation();
                break;
            case 'c':
                plotcolor();
                break;
            case 27:
                clear();
                mvprintw(10,20, "View graphs. p : population, c: color, g : gender, s : speed, Q : quit program");
                break;
            case 'Q':
                inprogram = 0;
            default:
                mvprintw(0,1, "Unknown command, try again. p : population, c: color, g : gender, s : speed, Q : quit program, esc : go back");
                break;

        }
    }

    endwin();

    return 0;
}

void print_terrain() {
    // Print the terrain
    int i, j;
    for (i = 0; i < HEIGHT; i++) {
        for (j = 0; j < WIDTH; j++) {
            // Print rabbits first
            if (rabbits[i][j]->symbol != ' ') {
                attron(COLOR_PAIR(rabbits[i][j]->color));
                mvaddch(1 + i, 20 + j, rabbits[i][j]->symbol);
                attroff(COLOR_PAIR(rabbits[i][j]->color));
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
    mvprintw(0, 35, "Rabbit Population graphed over time      (esc to go back)");
    //add y-axis labels
    int count = 0;
    for (int i : yaxismarks){
        mvprintw(HEIGHT - count, 5, "%d", i);
        count ++;
    }

    refresh();  
}

// plots the graph of the rabbit population, scaled to terminal size
void plotcolor() {

    clear();

    int maxval = *std::max_element(yellowrabbits.begin(), yellowrabbits.end()); //gets max yellow rabbits
    int maxval2 = *std::max_element(whiterabbits.begin(), whiterabbits.end()); //gets max white rabbits
    if (maxval2 > maxval){
        maxval = maxval2;
    }
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

    //plot points for yellow rabbits
    for (size_t d = 0; d < yellowrabbits.size(); d++) { 

        int currentpoint = yellowrabbits[d];
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
        attron(COLOR_PAIR(4));
        mvaddch(HEIGHT - tickrow, 20 + d, '*'); 
        attroff(COLOR_PAIR(4));
    }
    //plot points for white rabbits
    for (size_t d = 0; d < whiterabbits.size(); d++) { 

        int currentpoint = whiterabbits[d];
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
    mvprintw(0, 35, "Rabbit Colors graphed over time      (esc to go back)");
    //add y-axis labels
    int count = 0;
    for (int i : yaxismarks){
        mvprintw(HEIGHT - count, 5, "%d", i);
        count ++;
    }

    refresh();  
}

void hungerandthirst(){
    for (Rabbit* r : rabbitlist){
        int index = 0;
        int prevhunger = r->hunger;
        int prevthirst = r->thirst;
        if (terrain[r->y][r->x].symbol!='#'){
            r->hunger -= 10;
        }
        //if there isn't water nearby, the rabbit gets thirsty
        int i,j;
        int drank = 0;
        for (i=r->y-1; i < r->y+2 && i < HEIGHT; i++){
            for (j=r->x-1; j < r->x + 2 && j < WIDTH; j++){
                if (i>= 0 && j>= 0 && terrain[i][j].symbol!='~'){
                    r->thirst -= 10;
                    drank = 1;
                    break;
                }
            }
            if (drank == 1){
                break;
            }
        }
        if (prevthirst <= 0 || prevhunger <= 0){
            rabbits[rabbitlist[index]->y][rabbitlist[index]->x]->symbol = ' ';
            if (rabbits[rabbitlist[index]->y][rabbitlist[index]->x]->color == 4){
                numyellow--;
            } else {
                numwhite--;
            }
            rabbitlist.erase(rabbitlist.begin() + index);
            numrabbits --;
            index--;
        }

        index ++;
        //check for nearby water. Drink if there is a water block next to the rabbit.
        // int i, j;
        for (i=r->y-1; i <= r->y + 1 && i < HEIGHT; i++){
            for (j=r->x-1; j <= r->x + 1 && j < WIDTH; j++){
                if ( i>= 0 && j >= 0 && terrain[i][j].symbol=='~'){
                    r->thirst = 100; //they can drink as much as they want
                }
            }
        }
        //check if the rabbit is on a berry bush. If it is, eat. 
        if (terrain[r->y][r->x].symbol=='#'){
            r->hunger += 50;
            if (r->hunger > 100){ //don't let hunger go over 100
                r->hunger = 100;
            }
        }
    }
}
// ages up rabbits if the day is a multiple of 8. 8 days = 1 year. Rabbits can die of age years 7->10 w/ chances worsening as it goes on.
void agerabbits(int day){
    //8 days is a year
    // if (day - r->dob % 8 == 0){
    int index = 0;
    for (Rabbit* r : rabbitlist){
        if ((day - r->dob) % 8 == 0){
            r->age += 1;
        }
        if (r->age >= 1){
            r->symbol = 'R';
        }
        // mvprintw(1,index, "%d ", r->age);
        //kill rabbits if they gotta go
        if (r->age >= 10){
            rabbits[rabbitlist[index]->y][rabbitlist[index]->x]->symbol = ' ';
            if (rabbits[rabbitlist[index]->y][rabbitlist[index]->x]->color == 4){
                numyellow--;
            } else {
                numwhite--;
            }
            rabbitlist.erase(rabbitlist.begin() + index);
            numrabbits --;
            index--;
        } else if (r->age >= 9){
            int percentdeath = rand() % 11;
            if (percentdeath > 7){
                rabbits[rabbitlist[index]->y][rabbitlist[index]->x]->symbol = ' ';
                if (rabbits[rabbitlist[index]->y][rabbitlist[index]->x]->color == 4){
                    numyellow--;
                } else {
                    numwhite--;
                }
                rabbitlist.erase(rabbitlist.begin() + index);
                numrabbits--;
                index--;
            }
        } else if (r->age >= 8){
            int percentdeath = rand() % 11;
            if (percentdeath > 5){
                rabbits[rabbitlist[index]->y][rabbitlist[index]->x]->symbol = ' ';
                if (rabbits[rabbitlist[index]->y][rabbitlist[index]->x]->color == 4){
                    numyellow--;
                } else {
                    numwhite--;
                }
                rabbitlist.erase(rabbitlist.begin() + index);
                numrabbits--;
                index--;
            }
        } else if (r->age >= 7){
            int percentdeath = rand() % 11;
            if (percentdeath > 3){
                rabbits[rabbitlist[index]->y][rabbitlist[index]->x]->symbol = ' ';
                if (rabbits[rabbitlist[index]->y][rabbitlist[index]->x]->color == 4){
                    numyellow--;
                } else {
                    numwhite--;
                }
                rabbitlist.erase(rabbitlist.begin() + index);
                numrabbits--;
                index--;
            }
        }
        index++;
    }
    // }
}

void reproduce(){
    for (Rabbit* r : rabbitlist){
        //check surrounding tiles
        char currentgender = r->gender;
        char neededgender = 'M';
        if (currentgender=='M'){
            neededgender = 'F';
        }
        int i,j;
        for (i = r->y-1; i < r->y+2 && i < HEIGHT; i++){
            for (j=r->x-1; j < r->x+2 && j < WIDTH; j++){
                if (i>= 0 && j >= 0 && (i != r->y || j != r->x)){
                    if (rabbits[i][j]->symbol=='R' && r->symbol=='R' && rabbits[i][j]->gender==neededgender){
                        //then we can reproduce
                        if (r->gender=='F'){
                            r->pregnancy = 1;
                            r->colorpassdown = rabbits[i][j]->color;
                            // mvprintw(0,0, "Rabbit is pregnant");
                        } else {
                            rabbits[i][j]->pregnancy = 1;
                            rabbits[i][j]->colorpassdown = r->color;
                            // mvprintw(0,0, "Rabbit is pregnant");
                        }
                    }
                }
            }
        }
    }
}

void rabbitsborn(int day){
    //mother gives birth to as many babies as she can in her vicinity, as the average litter size is about 5-7 anyways
    for (Rabbit* r: rabbitlist){
        if (r->pregnancy == 1){
            int i,j;
            char randgender = 'M';
            int randgenderchance;
            for (i=r->y-1; i <r->y+2 && i < HEIGHT; i++){
                for (j=r->x-1; j < r->x+2 && j < WIDTH; j++){
                    if (i>= 0 && j >= 0 && terrain[i][j].symbol!='~' && rabbits[i][j]->symbol==' '){
                        // delete rabbits[i][j];
                        randgenderchance = rand() % 2;
                        if (randgenderchance==0){
                            randgender = 'F';
                        } else {
                            randgender='M';
                        }
                        int colorchance = rand() % 2;
                        if (colorchance==1){
                            colorchance = r->color;
                        } else {
                            colorchance = r->colorpassdown;
                        }
                        //for now inherit qualities of mother
                        rabbits[i][j]->color = colorchance;
                        rabbits[i][j]->gender = randgender;
                        rabbits[i][j]->speed = 1.0;
                        rabbits[i][j]->sightradius = 1;
                        rabbits[i][j]->symbol='r';
                        rabbits[i][j]->hunger = 100;
                        rabbits[i][j]->thirst = 100;
                        rabbits[i][j]->pregnancy = 0;
                        rabbits[i][j]->age = 0;
                        rabbits[i][j]->colorpassdown = r->color;
                        rabbits[i][j]->x = j;
                        rabbits[i][j]->y = i;
                        rabbits[i][j]->dob = day;
                        //  = Rabbit(r->color, randgender, 1.0, 1, 'r', 100, 100, 0, 0, i, j);
                        newborns.push_back(rabbits[i][j]);
                        numrabbits ++;
                        if (rabbits[i][j]->color == 4){
                            numyellow++;
                        } else {
                            numwhite++;
                        }
                    }
                }
            }

            r->pregnancy = 0;
        }
    }

    for (Rabbit* r : newborns){
        rabbitlist.push_back(r);
    }
    newborns.clear();
}









