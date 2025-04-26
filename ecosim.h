#ifndef ECOSIM_H
#define ECOSIM_H

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
        int speed; // how many blocks it can move. .5->2. if it isn't 1 or 2 then it takes that percentage chance of moving up. 
                   // So 1.3 has 30% chance of being able to move 2 spots.
        int sightradius;
        char symbol; //r for baby < 10 age. R for adult > 10 age. 
        // stats
        int hunger;
        int thirst;
        int age;
        int pregnancy; //tracks how many days pregnant a rabbit is
        Rabbit(): color(-1), gender('X'), speed(-1), sightradius(-1), symbol('X'), hunger(-1), thirst(-1), age(-1), pregnancy(-1){
        }
        Rabbit(int c, char g, int sp, int si, char sym, int h, int t, int a, int p){
            color = c;
            gender = g;
            speed = sp;
            sightradius = si;
            symbol = sym;
            hunger = h;
            thirst = t;
            age = a;
            pregnancy = p;
        }

};

extern int numrabbits;
// [numrows][numcols]
extern land terrain[HEIGHT][WIDTH];
extern Rabbit rabbits[HEIGHT][WIDTH];

void generate_terrain(); 
void spawnRabbits();




#endif