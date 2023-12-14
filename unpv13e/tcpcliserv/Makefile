include ../Make.defines

PROGS =	client server

all:	${PROGS}

# Rules for building client and server
client:	client.o
		${CC} ${CFLAGS} -o $@ client.o ${LIBS}

server:	server.o
		${CC} ${CFLAGS} -o $@ server.o ${LIBS}

# Clean rule
clean:
		rm -f ${PROGS} ${CLEANFILES} *.o
