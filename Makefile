run:
	gcc -lncurses src/main.c -o build/run -lncurses && ./build/run

comp:
	gcc -lncurses src/main.c -o build/run -lncurses 
