# template for makefile

SRC_NAME = afp2xml.c
PRG_NAME =$(SRC_NAME:.c=.exe)

.PHONY: all run test_afpexplorer test_hex_afp

all:
	$(MAKE) -C src

run: all
	./bin/$(PRG_NAME) ./test1/test.afp | xmllint --format - | head -n 40

clean:
	rm -f ./bin/$(PRG_NAME)
	rm -f ./tmp/*

test_afpexplorer: all
	mkdir -p tmp
	./bin/$(PRG_NAME) ./test1/test_01.afp | xsltproc xslt/get_data.xslt - > tmp/test_xslt.txt
	iconv -c -f "utf-8" -t "ascii" test1/test_01.txt | tr -dc '[[:print:]]\n' > tmp/test_01.txt
	diff -s tmp/test_xslt.txt tmp/test_01.txt

test_hex_afp: all
	mkdir -p tmp
	./bin/$(PRG_NAME) ./test1/test_01.afp | xsltproc xslt/get_hex.xslt - > tmp/test_xslt.hex
	xxd -g 0 -p test1/test_01.afp | tr -d "\n" > tmp/test_01_afp.hex
	diff -s tmp/test_xslt.hex tmp/test_01_afp.hex
