# template for makefile

SRC_NAME = afp_dump.c
PRG_NAME =$(SRC_NAME:.c=.exe)

.PHONY: all run

all:
	$(MAKE) -C src

run: all
	./bin/$(PRG_NAME) ./test1/test.afp | xmllint --format - | head -n 40

clean:
	rm -f ./bin/$(PRG_NAME)
