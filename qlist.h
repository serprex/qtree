#pragma once
#include "qtype.h"
qlist*qlnew();
void qldel(qlist*n);
void qladd(qlist**n,void*q);
int qlin(qlist*n,void*q);
void qlset(qlist*n,uint32_t i);
void*qlnxt(qlist*n);