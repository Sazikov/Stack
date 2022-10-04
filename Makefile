FLAGS=-Wall -Wextra -Wpedantic

all:
	g++ $(FLAGS) main.cpp stack.cpp -o main