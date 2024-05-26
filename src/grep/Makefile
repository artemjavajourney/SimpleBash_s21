CC = gcc $(FLAGS) 
FLAGS = -Wall -Werror -Wextra -std=c11 -pedantic



all: s21_grep

s21_grep: s21_grep.c
		$(CC) s21_grep.c -o s21_grep

test: s21_grep
		chmod 777 tests.sh
		bash tests.sh

rebuild: 
		make clean
		make all


clean:
		rm -rf s21_grep
