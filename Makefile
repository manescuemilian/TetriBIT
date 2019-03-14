
.PHONY: run

build:
	gcc tetris.c -o tema1 -Wall -lm
	
run:	build
	./tema1

clean:
	rm -rf tema1
