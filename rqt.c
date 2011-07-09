#include "rqtree.h"
#include <stdlib.h>
rqtree*rqtnew(void){return calloc(sizeof(rqtree),1);}
void rqtdel(rqtree*q){
	for(int i=0;i<4;i++)
		if(q->n[i])rqtdel(q->n[i]);
	free(q);
}
static rqtree*qtadd__(rqtree**qq,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2){
	rqtree*q=*qq=realloc(*qq,8*++(*qq)->x);
	q->r[q->x-4]=x1;
	q->r[q->x-3]=y1;
	q->r[q->x-2]=x2;
	q->r[q->x-1]=y2;
	return q;
}
static rqtree*qtadd_(rqtree*q,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint16_t X1,uint16_t Y1,uint16_t X2,uint16_t Y2,rqtree**h){
	uint16_t x12=X1+X2>>1,y12=Y1+Y2>>1;
	if(x1<x12&&x2>x12||y1<y12&&y2>y12)return qtadd__(h,x1,y1,x2,y2);
	if(x1>x12){
		if(y1>y12){
			if(!q->n[0])q->n[0]=rqtnew();
			return qtadd_(q->n[0],x1,y1,x2,y2,x12,y12,X2,Y2,q->n);
		}else{
			if(!q->n[1])q->n[1]=rqtnew();
			return qtadd_(q->n[1],x1,y1,x2,y2,x12,Y1,X2,y12,q->n+1);
		}
	}else if(y1>y12){
		if(!q->n[2])q->n[2]=rqtnew();
		return qtadd_(q->n[2],x1,y1,x2,y2,X1,y12,x12,Y2,q->n+2);
	}else{
		if(!q->n[3])q->n[3]=rqtnew();
		return qtadd_(q->n[3],x1,y1,x2,y2,X1,Y1,x12,y12,q->n+3);
	}
}
rqtree*rqtadd(rqtree*q,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2){return qtadd_(q,x1,y1,x2,y2,0,0,65535,65535,&q);}
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