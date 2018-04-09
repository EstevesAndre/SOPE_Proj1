#Makefile for simgrep

simgrep: simgrep.c
	gcc -o simgrep -Wall -std=gnu11 simgrep.c
