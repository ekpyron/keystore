CC=gcc
CFLAGS=$(shell pkg-config --cflags gtk+-3.0 libconfig)
LIBS=$(shell pkg-config --libs gtk+-3.0 libconfig)

all: keystore

clean:
	rm -f *.o keystore

keystore: keystore.o
	${CC} -o keystore keystore.o ${LIBS}

keystore.o: keystore.c
	${CC} -c -o keystore.o keystore.c ${CFLAGS}

test: test.c
	${CC} -o test test.c ${LIBS} ${CFLAGS}
