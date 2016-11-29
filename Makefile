CC = gcc
FLAGS = -Wall -g

all: vig

vig: vig.c
	$(CC) $(FLAGS) -o vig vig.c
