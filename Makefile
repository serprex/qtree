ifneq ($(GLX),)
CFLAGS=-DGLX
else
CFLAGS=-DSDL `sdl-config --cflags` `sdl-config --libs`
endif
a:qt test
qt:qt.c
	gcc -std=gnu99 -Os -march=native -s -ffast-math -ftree-vectorize -o qtree.o qt.c -c
test:test.c qt
	gcc -std=gnu99 -Os -march=native -s -ffast-math -fwhole-program -ftree-vectorize -o test test.c qtree.o ${CFLAGS} -lGL