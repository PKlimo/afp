# template for makefile

CC=gcc
CFLAGS=-std=c99 -Wall -Werror -pedantic -pedantic-errors -Wextra -Wshadow -Wpointer-arith -Wcast-qual \
        -Wstrict-prototypes -Wmissing-prototypes
CFLAGS += -O2
SRC_NAME = afp_dump.c
SOURCES = afp_extract_function.h ebcdic.h
PRG_NAME = ../bin/$(SRC_NAME:.c=.exe)

all: $(PRG_NAME)

$(PRG_NAME): $(SRC_NAME) $(SOURCES)
	$(CC) $(CFLAGS) $(SRC_NAME) -o $(PRG_NAME)

run: $(PRG_NAME)
	./$(PRG_NAME) ../test1/test.afp | xmllint --format - | head -n 40

clean:
	rm -f $(PRG_NAME)
