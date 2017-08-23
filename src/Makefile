CC=g++
CFLAGS=-I../include --std=c++1y -c
HEADERS=../include/controls.h ../include/field.h ../include/figure.h ../include/figure_position.h ../include/utility_func.h

all: tetris

tetris: utility_func.o figure.o figure_position.o all_figure_positions.o field.o main.o
	$(CC) -o tetris utility_func.o figure.o figure_position.o all_figure_positions.o field.o main.o -lX11

main.o: main.cpp $(HEADERS)
	$(CC) $(CFLAGS) main.cpp

utility_func.o: utility_func.cpp $(HEADERS)
	$(CC) $(CFLAGS) utility_func.cpp

field.o: field.cpp $(HEADERS)
	$(CC) $(CFLAGS) field.cpp

figure.o: figure.cpp $(HEADERS)
	$(CC) $(CFLAGS) figure.cpp

figure_position.o: figure_position.cpp $(HEADERS)
	$(CC) $(CFLAGS) figure_position.cpp

all_figure_positions.o: all_figure_positions.cpp $(HEADERS)
	$(CC) $(CFLAGS) all_figure_positions.cpp

clean:
	rm *.o tetris
