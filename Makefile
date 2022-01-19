PROGRAM = main

SRC_DIR = src/
SRC_EXT = .c

CFLAGS = $(shell mysql_config --cflags) $(shell pkg-config --cflags gtk+-3.0) $(shell curl-config --cflags) -I./headers
LIBS = $(shell mysql_config --libs) $(shell pkg-config --libs gtk+-3.0) $(shell curl-config --libs)


FILES = $(wildcard $(SRC_DIR)*.c)


install: 
	apt-get install mysql

prog:
	@gcc -o $(PROGRAM) $(CFLAGS) $(FILES) $(LIBS)
	./main

newprog:
	gcc -o $(PROGRAM) $(CFLAGS) $(FILES) $(LIBS)
	konsole --hold -e "./main"
