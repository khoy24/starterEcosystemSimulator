dungeon: ecosim.o 
	g++ ecosim.cpp -o ecosim -Wall -Werror -lm -lncurses

# -lm links math library for use of sqrtf
ecosim.o: ecosim.cpp ecosim.h
	g++ -g ecosim.cpp -o ecosim -Wall -Werror -lm -c

clean: 
	rm -f *.o *~ ecosim
