FLAGS=-Wall -Wextra -Wpedantic

all:
	g++ $(FLAGS) main.cpp src\stack.cpp -o main