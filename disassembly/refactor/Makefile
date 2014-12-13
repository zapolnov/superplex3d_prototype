
all: supaplex

supaplex: supaplex_logic.o supaplex.o BMPLoader.o
	g++ -g -o $@ $^ -lGL -lglut -lrt

supaplex.o: supaplex.asm
	yasm -DTASM -g dwarf2 -o $@ -p tasm -f elf $<

supaplex_logic.o: supaplex_logic.cpp
	g++ -DUSE_GLUT -g -c -o $@ $<

BMPLoader.o:
	g++ -g -c -o BMPLoader.o BMPLoader.cpp

clean:
	rm -f supaplex supaplex.o supaplex_logic.o BMPLoader.o
