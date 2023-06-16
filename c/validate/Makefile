TARGET=validate_ones_zeros
OBJS=validate_ones_zeros.o ones_zeros_model.o
CFLAGS=-Wall -Wextra -Werror -g
LINKS=-lm

all: ${TARGET}

${TARGET}: ${OBJS}
	gcc -o ${TARGET} ${OBJS} ${CFLAGS} ${LINKS}

#the - ignores errors
clean:
	-rm ${TARGET} ${OBJS}
