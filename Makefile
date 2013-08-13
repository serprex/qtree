ifneq ($(GLX),)
CF=-DGLX -lX11
else
CF=-DSDL `sdl2-config --cflags` `sdl2-config --libs`
endif
CF+=-fwhole-program -lGL -L. -lqtree
CFLAGS=-std=gnu99 -O2 -march=native -s -ffast-math -fPIC
a:ptest rtest
qt:rqt.o pqt.o ql.o
	${CC} -std=gnu11 -shared -o libqtree.so pqt.o rqt.o ql.o
ptest:ptest.c qt
	${CC} -std=gnu11 -o ptest ptest.c ${CF}
rtest:rtest.c qt
	${CC} -std=gnu11 -o rtest rtest.c ${CF}
test:ptest rtest
	LD_LIBRARY_PATH=. ./ptest
	LD_LIBRARY_PATH=. ./rtest
	rm *.o