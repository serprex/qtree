#include <stdint.h>
typedef struct qtree{
	struct qtree*n[4];
	uint16_t x,y;
}qtree;
typedef struct qlist{
	struct qlist*n;
	qtree*q;
}qlist;
qlist*qlnew(qtree*);
void qldel(qlist*);
void qladd(qlist**,qtree*);
qtree*qtnew(uint16_t,uint16_t);
void qtdel(qtree*);
qtree*qtadd(qtree*,uint16_t,uint16_t);
void qtsub(qtree*,uint16_t,uint16_t);
qtree*qtget(qtree*,uint16_t,uint16_t);
qtree*qtcpy(qtree*);
uint32_t qtlen(qtree*);
qtree*qtnear(qtree*,uint16_t,uint16_t);
qlist*qtsnear(qtree*,uint16_t,uint16_t,uint32_t);
qlist*qtsznear(qtree*,uint16_t,uint16_t,uint32_t,qlist*);