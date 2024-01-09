include ../Make.defines

PROGS =	client server cli

all:	${PROGS}

# Rules for building client and server
client:	client.o
		${CC} ${CFLAGS} -o $@ client.o ${LIBS}

cli:	cli.o
		${CC} ${CFLAGS} -o $@ cli.o ${LIBS}

server:	server.o
		${CC} ${CFLAGS} -o $@ server.o ${LIBS}

# Clean rule
clean:
		rm -f ${PROGS} ${CLEANFILES}
