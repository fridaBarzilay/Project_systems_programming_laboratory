run: main.o macro.o first_trans.o second_trans.o help_func.o checks.o
	gcc -ansi -Wall -pedantic -g main.o macro.o first_trans.o second_trans.o help_func.o checks.o -o run
main.o:main.c macro.h first_trans.h help_func.h encode.h
	gcc -ansi -Wall -pedantic -c main.c -o main.o
macro.o:macro.c macro.h
	gcc -ansi -Wall -pedantic -c macro.c -o macro.o
first_trans.o:first_trans.c first_trans.h help_func.h encode.h
	gcc -ansi -Wall -pedantic -c first_trans.c -o first_trans.o
second_trans.o:second_trans.c second_trans.h help_func.h encode.h
	gcc -ansi -Wall -pedantic -c second_trans.c -o second_trans.o	
help_func.o:help_func.c help_func.h
	gcc -ansi -Wall -pedantic -c help_func.c -o help_func.o
checks.o:checks.c checks.h
	gcc -ansi -Wall -pedantic -c checks.c -o checks.o

