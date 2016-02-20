OBJS = server.o insert_db.o select_db.o mailin_db.o infoout_db.o textout_db.o change_db.o
TARGET = server
CFLAG =    -g -Wall -O2 -lsqlite3
CC = gcc
all: server

$(TARGET): server.o insert_db.o select_db.o mailin_db.o infoout_db.o textout_db.o change_db.o
	$(CC) $(CFLAG) -o server server.o insert_db.o select_db.o mailin_db.o infoout_db.o textout_db.o change_db.o

server.o:server.c
	$(CC) $(CFLAG) -o server.o -c    server.c -I./include
insert_db.o:insert_db.c
	$(CC) $(CFLAG) -o insert_db.o -c    insert_db.c -I./include
select_db.o:select_db.c
	$(CC) $(CFLAG) -o select_db.o -c    select_db.c -I./include
mailin_db.o:mailin_db.c
	$(CC) $(CFLAG) -o mailin_db.o -c    mailin_db.c -I./include
infoout_db.o:infoout_db.c
	$(CC) $(CFLAG) -o infoout_db.o -c   infoout_db.c -I./include
textout_db.o:textout_db.c
	$(CC) $(CFLAG) -o textout_db.o -c   textout_db.c -I./include
change_db.o:change_db.c
	$(CC) $(CFLAG) -o change_db.o -c   change_db.c -I./include
clean:
	rm -rf $(OBJS) $(TARGET)
