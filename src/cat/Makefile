CC = gcc $(FLAGS) 
FLAGS = -Wall -Werror -Wextra -std=c11 -pedantic



all: s21_cat

s21_cat: s21_cat.c
		$(CC) s21_cat.c -o s21_cat

test: s21_cat
		chmod 777 tests.sh
		bash tests.sh

rebuild: 
		make clean
		make all


clean:
		rm -rf s21_cat
