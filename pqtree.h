#pragma once
#include "qlist.h"
pqtree*pqtnew(uint16_t,uint16_t);
void pqtdel(pqtree*);
qlist*pqtql(pqtree*);
pqtree*pqtadd(pqtree*,uint16_t,uint16_t);
void pqtsub(pqtree*,uint16_t,uint16_t);
pqtree*pqtget(pqtree*,uint16_t,uint16_t);
pqtree*pqtcpy(pqtree*);
uint32_t pqtlen(pqtree*);
pqtree*pqtnear(pqtree*,uint16_t,uint16_t);
pqtree*pqtznear(pqtree*,uint16_t,uint16_t,qlist*);
qlist*pqtsnear(pqtree*,uint16_t,uint16_t,uint32_t);
qlist*pqtsznear(pqtree*,uint16_t,uint16_t,uint32_t,qlist*);