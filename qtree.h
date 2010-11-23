#include <stdlib.h>
#include <stdint.h>
typedef struct qtree{
	struct qtree*n[5];
	union{
	void*p;
	intptr_t d;
	};
	uint16_t x,y;
}qtree;
qtree*qtnew(uint16_t,uint16_t);
void qtdel(qtree*);
qtree*qtadd(qtree*,uint16_t,uint16_t);
void qtsubdir(qtree*);
void qtsub(qtree*,uint16_t,uint16_t);
qtree*qtget(qtree*,uint16_t,uint16_t);
qtree*qtcpy(qtree*);
unsigned qtlen(qtree*);
unsigned qtdepx(qtree*);
unsigned qtdepn(qtree*);
qtree*qtnearxy(qtree*,uint16_t,uint16_t);
qtree*qtnear(qtree*);