CC=g++
CFLAGS=-I../include --std=c++1y -c 

all: tetris

tetris: utility_func.o figure.o figure_position.o all_figure_positions.o field.o main.o
	$(CC) -o tetris utility_func.o figure.o figure_position.o all_figure_positions.o field.o main.o

main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp

utility_func.o: utility_func.cpp
	$(CC) $(CFLAGS) utility_func.cpp

field.o: field.cpp
	$(CC) $(CFLAGS) field.cpp

figure.o: figure.cpp
	$(CC) $(CFLAGS) figure.cpp

figure_position.o: figure_position.cpp
	$(CC) $(CFLAGS) figure_position.cpp

all_figure_positions.o: all_figure_positions.cpp
	$(CC) $(CFLAGS) all_figure_positions.cpp

clean:
	rm *.o tetris