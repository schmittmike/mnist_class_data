TARGET=read_write_image
OBJS=read_write_image.o
CFLAGS=-Wall -Wextra -Werror -g
LINKS=-lm

all: ${TARGET}

${TARGET}: ${OBJS}
	gcc -o ${TARGET} ${OBJS} ${CFLAGS} ${LINKS}

#the - ignores errors
clean:
	-rm ${TARGET} ${OBJS}
