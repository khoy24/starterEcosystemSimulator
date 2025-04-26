ecosim: ecosim.o terrain.o
	g++ ecosim.cpp terrain.cpp -o ecosim -Wall -Werror -lm -lncurses

# -lm links math library for use of sqrtf
ecosim.o: ecosim.cpp ecosim.h
	g++ -g ecosim.cpp -o ecosim -Wall -Werror -lm -c

terrain.o: terrain.cpp
	g++ -g terrain.cpp -o terrain -Wall -Werror -lm -c

clean: 
	rm -f *.o *~ ecosim terrain
