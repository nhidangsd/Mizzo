#James Lee (RedID: 820655947)

mizzo: mizzo.o producers.o belt.o consumers.o io.o
	g++ -o mizzo mizzo.o producers.o belt.o consumers.o io.o -lpthread -lrt -g

mizzo.o: mizzo.cpp
	g++ -c -g mizzo.cpp -lpthread -lrt -std=c++11

io.o: io.c io.h production.h
	g++ -c -w io.c -std=c++11

producers.o: producers.cpp
	g++ -c producers.cpp

consumers.o: consumers.cpp
	g++ -c consumers.cpp

belt.o: belt.cpp
	g++ -c belt.cpp

clean:
	rm *.o
	rm mizzo