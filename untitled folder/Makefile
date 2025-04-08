EXEC = programa
SRCS = main.c processo.c
CC = gcc
CFLAGS = -Wall -std=c99

all: $(EXEC)

$(EXEC): $(SRCS)
	$(CC) $(CFLAGS) -o $(EXEC) $(SRCS)

clean:
	rm -f $(EXEC)

run: $(EXEC)
	./$(EXEC)