#ifndef ECOSIM_H
#define ECOSIM_H

#include <vector>

const int HEIGHT = 20;
const int WIDTH = 80;

class land {
    public:
        int color;
        char symbol;
};

class Rabbit {
    public: 
        // traits
        int color; // 0 white, 4 yellow
        char gender; // M = male, F = female
        double speed; // how many blocks it can move. .5->2. if it isn't 1 or 2 then it takes that percentage chance of moving up. 
                   // So 1.3 has 30% chance of being able to move 2 spots.
        int sightradius;
        char symbol; //r for baby < 10 age. R for adult > 10 age. 
        // stats
        int hunger;
        int thirst;
        int age;
        int pregnancy; //tracks how many days pregnant a rabbit is
        int colorpassdown;
        int dob;

        //location
        int y;
        int x;

        Rabbit(): color(-1), gender('X'), speed(-1.0), sightradius(-1), symbol(' '), hunger(-1), thirst(-1), age(-1), pregnancy(-1), colorpassdown(0), dob(0){
        }
        Rabbit(int c, char g, int sp, int si, char sym, int h, int t, int a, int p, int cp, int bday, int yco, int xco){
            color = c;
            gender = g;
            speed = sp;
            sightradius = si;
            symbol = sym;
            hunger = h;
            thirst = t;
            age = a;
            pregnancy = p;
            colorpassdown = cp;
            dob = bday;
            y = yco;
            x = xco;

        }

};

extern int numrabbits;
extern int numfemales;
extern int nummales;
extern int numyellow;
extern int numwhite;
extern double averagespeed;
// [numrows][numcols]
extern land terrain[HEIGHT][WIDTH];
extern Rabbit* rabbits[HEIGHT][WIDTH];

extern std::vector<Rabbit*> rabbitlist;
extern std::vector<int> points;
extern std::vector<int> days;

void generate_terrain(); 
void spawnRabbits();
void moveRabbits();
void updaterabbits();


#endif