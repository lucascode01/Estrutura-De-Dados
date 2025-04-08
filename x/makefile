CC = gcc
CFLAGS = -Wall -Wextra -g3
OUTPUT_DIR = output
OUTPUT_BIN = $(OUTPUT_DIR)/main

SRC = main.c processo.c
HEADERS = processo.h

all: $(OUTPUT_BIN)

$(OUTPUT_BIN): $(SRC) $(HEADERS)
	mkdir -p $(OUTPUT_DIR)
	$(CC) $(CFLAGS) $(SRC) -o $(OUTPUT_BIN)

run: all
	./$(OUTPUT_BIN)

clean:
	rm -rf $(OUTPUT_DIR)