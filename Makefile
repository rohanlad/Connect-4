CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11
DEPS = add.h subtract.h

all: maths_prog.o add.o subtract.o create_library link_dynamically
	$(CC) $(CFLAGS) -o maths_prog maths_prog.o add.o subtract.o

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

create_library: add.o subtract.o
	$(CC) $(CFLAGS) -fPIC -c add.c -o add.o
	$(CC) $(CFLAGS) -fPIC -c subtract.c -o subtract.o
	$(CC) $(CFLAGS) -shared add.o subtract.o -o libmaths.so

link_dynamically: maths_prog.o libmaths.so
	$(CC) $(CFLAGS) maths_prog.o -L./ -lmaths -o maths_prog

clean:
	rm -rf *.o maths_prog libmaths.so

 