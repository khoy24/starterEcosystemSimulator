#ifndef ECOSIM_H
#define ECOSIM_H

const int HEIGHT = 20;
const int WIDTH = 80;

class land {
    public:
        int color;
        char symbol;
};
// [numrows][numcols]
extern land terrain[HEIGHT][WIDTH];

void generate_terrain(); 




#endif