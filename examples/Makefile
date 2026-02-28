SRC := main.c
STD ?= c99

OUT := main
INCLUDE := -I../

build: $(SRC)
	cc -Wall -Wextra -std=$(STD) $(INCLUDE) -pedantic $(SRC) -o $(OUT)

clean: 
	rm -f $(OUT)
