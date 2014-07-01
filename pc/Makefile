CC = clang
OBJS = communication.o main.o

all: $(OBJS)
	$(CC) -o pc $(OBJS) -Wall -Werror

clean:
	rm -f *.o pc