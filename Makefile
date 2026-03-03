#################################################################
#
#  NAME:        Dominic Molina
#
#  HOMEWORK:    Program 2
#
#  CLASS:       ICS 451
#
#  INSTRUCTOR:  Ravi Narayan
#
#  DATE:        February 24, 2025
#
#  FILE:        Makefile
#
#  DESCRIPTION: Compiles to create the executable
#
#
#
#################################################################
server: server.o
	gcc -o server server.o

server.o: server.c
	gcc -ansi -pedantic-errors -Wall -c server.c

clean:
	rm -f server server.o