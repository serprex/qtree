#pragma once
#include "qlist.h"
rqtree*rqtnew(void);
void rqtdel(rqtree*);
rqtree*rqtadd(rqtree*,uint16_t,uint16_t,uint16_t,uint16_t);
void rqtsubp(rqtree*,uint16_t,uint16_t);
void rqtsubr(rqtree*,uint16_t,uint16_t,uint16_t,uint16_t);
qlist*rqtget(rqtree*,uint16_t,uint16_t);
uint32_t rqtlen(rqtree*);