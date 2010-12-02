ifneq ($(GLX),)
CF=-DGLX
else
CF=-DSDL `sdl-config --cflags` `sdl-config --libs`
endif
CF+=-fwhole-program -lGL -L. -lqtree
CC=gcc -std=gnu99 -O2 -march=native -s -ffast-math -fPIC
a:ptest rtest
qt:rqt.o pqt.o ql.o
	${CC} -shared -o libqtree.so pqt.o rqt.o ql.o
ptest:ptest.c qt
	${CC} -o ptest ptest.c ${CF}
rtest:rtest.c qt
	${CC} -o rtest rtest.c ${CF}
test:ptest rtest
	LD_LIBRARY_PATH=. ./ptest
	LD_LIBRARY_PATH=. ./rtest
	rm *.o