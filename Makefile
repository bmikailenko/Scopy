CPP = gcc
FLAGS = -Wall -std=c99 -pedantic -g

EXEC = scopy
OBJS = scopy.o

default:${EXEC}

clean:
	rm -f ${EXEC}
	rm -f *.o

run: ${EXEC}
	./${EXEC}

${EXEC}:${OBJS}
	${CPP} ${FLAGS} -o ${EXEC} ${OBJS}

.c.o:
	${CPP} ${FLAGS} -c $<

scopy.o: scopy.c
