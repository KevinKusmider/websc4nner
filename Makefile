PROGRAM = main

SRC_DIR = src/
SRC_EXT = .c

CFLAGS = $(shell mysql_config --cflags) $(shell pkg-config --cflags gtk+-3.0) $(shell curl-config --cflags) -I./headers
LIBS = $(shell mysql_config --libs) $(shell pkg-config --libs gtk+-3.0) $(shell curl-config --libs) -rdynamic

DB_USER = webscanner_user
DB_PASSWORD = Secure11
DB_NAME = webscanner

FILES = $(wildcard $(SRC_DIR)*.c)


install:
	apt-get install mysql
	apt-get install gtk3-dev
	apt-get install libcurl-dev

prog:
	@gcc -o $(PROGRAM) $(CFLAGS) $(FILES) $(LIBS) 
	@./main

newprog:
	gcc -o $(PROGRAM) $(CFLAGS) $(FILES) $(LIBS)
	konsole --hold -e "./main"


DB:
	$(shell mysql -uroot -psecure11 -e "CREATE DATABASE $(DB_NAME);")
	$(shell mysql -uroot -psecure11 -e "CREATE USER '$(DB_USER)'@'localhost' IDENTIFIED BY '$(DB_PASSWORD)'; GRANT ALL PRIVILEGES ON $(DB_NAME).* TO '$(DB_USER)'@'localhost';")
	$(shell mysql -u$(DB_USER) -p$(DB_PASSWORD) webscanner < database/reset/tables.sql)

deleteDB:
	$(shell mysql -uroot -psecure11 -e "DROP DATABASE $(DB_NAME);")
	$(shell mysql -uroot -psecure11 -e "DROP USER '$(DB_USER)'@'localhost';")

dumpDB:
	$(shell mysqldump -uroot -psecure11 files/dumps/dump.sql < webscanner");