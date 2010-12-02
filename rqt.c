#include "rqtree.h"
#include <stdlib.h>
rqtree*rqtnew(void){return calloc(sizeof(rqtree),1);}
static rqtree*rqtnew4(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2){
	rqtree*r=calloc(sizeof(rqtree)+8,1);
	r->r[0]=x1;
	r->r[1]=y1;
	r->r[2]=x2;
	r->r[3]=y2;
	return r;
}
void rqtdel(rqtree*q){
	for(int i=0;i<4;i++)
		if(q->n[i])rqtdel(q->n[i]);
	free(q);
}
static rqtree*qtadd__(rqtree*q,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2){
	q->r=realloc(q->r,8*++q->x);
	q->r[q->x-4]=x1;
	q->r[q->x-3]=y1;
	q->r[q->x-2]=x2;
	q->r[q->x-1]=y2;
	return q;
}
static rqtree*qtadd_(rqtree*q,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t X1,uint16_t Y1,uint16_t X2,uint16_t Y2){
	uint16_t x12=X1+X2>>1,y12=Y1+Y2>>1;
	if(x1<x12&&x2>x12||y1<y12&&y2>y12)return qtadd__(q,x1,y1,x2,y2);
	if(x1>x12){
		if(y1>y12)return q->n[0]?qtadd_(q->n[0],x1,y1,x2,y2,x12,y12,X2,Y2):(q->n[0]=rqtnew4(x1,y1,x2,y2));
		else return q->n[1]?qtadd_(q->n[1],x1,y1,x2,y2,x12,Y1,X2,y12):rqtnew4(x1,y1,x2,y2);
	}else if(y1>y12)return q->n[2]?qtadd_(q->n[2],x1,y1,x2,y2,X1,y12,x12,Y2):rqtnew4(x1,y1,x2,y2);
	else return q->n[3]?qtadd_(q->n[3],x1,y1,x2,y2,X1,Y1,x12,y12):rqtnew4(x1,y1,x2,y2);
}
rqtree*rqtadd(rqtree*q,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2){return qtadd_(q,x1,y1,x2,y2,0,0,65535,65535);}
void rqtsubp(rqtree*q,uint16_t x,uint16_t y){
}
void rqtsubr(rqtree*q,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2){
}
qlist*rqtgetp(rqtree*q,uint16_t x,uint16_t y){
	qlist*r=qlnew();
	return r;
}
uint32_t rqtlen(rqtree*q){
	uint32_t r=q->x;
	for(int i=0;i<4;i++)
		if(q->n[i])r+=rqtlen(q->n[i]);
	return r;
}