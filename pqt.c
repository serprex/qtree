#include "pqtree.h"
#include <stdlib.h>
static uint32_t sqr(uint32_t x){return x*x;}
static void qlqt_(pqtree*q,qlist*n){
	n->q[n->p++]=q;
	for(int i=0;i<4;i++)
		if(q->n[i])qlqt_(q->n[i],n);
}
qlist*pqlqt(pqtree*q){
	uint32_t ql=pqtlen(q);
	qlist*n=malloc(sizeof(qlist)+sizeof(pqtree*)*ql);
	n->n=ql;
	qlqt_(q,n);
	n->p=0;
	return n;
}
pqtree*pqtnew(uint16_t x,uint16_t y){
	pqtree*r=calloc(1,sizeof(pqtree));
	r->x=x;
	r->y=y;
	return r;
}
void pqtdel(pqtree*q){
	for(int i=0;i<4;i++)
		if(q->n[i])pqtdel(q->n[i]);
	free(q);
}
uint32_t pqtlen(pqtree*q){
	uint32_t r=1;
	for(int i=0;i<4;i++)
		if(q->n[i])r+=pqtlen(q->n[i]);
	return r;
}
static pqtree*qtadd_(pqtree*q,int x,int y,int x1,int y1,int x2,int y2){
	if(x==q->x&&y==q->y)return q;
	int x12=x1+x2>>1,y12=y1+y2>>1;
	if(x>x12){
		if(y>y12){
			if(q->n[0])return qtadd_(q->n[0],x,y,x12,y12,x2,y2);
			else return q->n[0]=pqtnew(x,y);
		}else if(q->n[1])return qtadd_(q->n[1],x,y,x12,y1,x2,y12);
		else return q->n[1]=pqtnew(x,y);
	}else if(y>y12){
		if(q->n[2])return qtadd_(q->n[2],x,y,x1,y12,x12,y2);
		else return q->n[2]=pqtnew(x,y);
	}else if(q->n[3])return qtadd_(q->n[3],x,y,x1,y1,x12,y12);
	else return q->n[3]=pqtnew(x,y);
}
pqtree*pqtadd(pqtree*q,uint16_t x,uint16_t y){return qtadd_(q,x,y,0,0,65535,65535);}
static pqtree*qtget_(pqtree*q,int x,int y,int x1,int y1,int x2,int y2){
	if(x==q->x&&y==q->y)return q;
	int x12=x1+x2>>1,y12=y1+y2>>1;
	if(x>x12){
		if(y>y12)return q->n[0]?qtget_(q->n[0],x,y,x12,y12,x2,y2):0;
		else return q->n[1]?qtget_(q->n[1],x,y,x12,y1,x2,y12):0;
	}else if(y>y12)return q->n[2]?qtget_(q->n[2],x,y,x1,y12,x12,y2):0;
	else return q->n[3]?qtget_(q->n[3],x,y,x1,y1,x12,y12):0;
}
pqtree*pqtget(pqtree*q,uint16_t x,uint16_t y){return qtget_(q,x,y,0,0,65535,65535);}
static void qtsub__(pqtree*q,pqtree**h){
	for(int i=0;i<4;i++)
		if(q->n[i]){
			q->x=q->n[i]->x;
			q->y=q->n[i]->y;
			qtsub__(q->n[i],q->n+i);
			return;
		}
	if(h){
		*h=0;
		free(q);
		return;
	}
}
static void qtsub_(pqtree*q,int x,int y,int x1,int y1,int x2,int y2,pqtree**h){
	if(x==q->x&&y==q->y)qtsub__(q,h);
	else{
		int x12=x1+x2>>1,y12=y1+y2>>1;
		if(x>x12){
			if(y>y12){
				if(q->n[0])qtsub_(q->n[0],x,y,x12,y12,x2,y2,q->n);
			}else if(q->n[1])qtsub_(q->n[1],x,y,x12,y1,x2,y12,q->n+1);
		}else if(y>y12){
			if(q->n[2])qtsub_(q->n[2],x,y,x1,y12,x12,y2,q->n+2);
		}else if(q->n[3])qtsub_(q->n[3],x,y,x1,y1,x12,y12,q->n+3);
	}
}
void pqtsub(pqtree*q,uint16_t x,uint16_t y){qtsub_(q,x,y,0,0,65535,65535,0);}
pqtree*pqtcpy(pqtree*q){
	pqtree*n=pqtnew(q->x,q->y);
	for(int i=0;i<4;i++)
		if(q->n[i])n->n[i]=pqtcpy(q->n[i]);
	return n;
}
static inline uint32_t pttosqdi(int x,int y,int x1,int y1,int x2,int y2){
	switch(x>=x1|(x<=x2)<<1|(y>=y1)<<2|(y<=y2)<<3){
	default:__builtin_unreachable();
	case 5:return sqr(x-x2)+sqr(y-y2);
	case 6:return sqr(x-x1)+sqr(y-y2);
	case 7:return sqr(y-y2);
	case 9:return sqr(x-x2)+sqr(y-y1);
	case 10:return sqr(x-x1)+sqr(y-y1);
	case 11:return sqr(y-y1);
	case 13:return sqr(x-x2);
	case 14:return sqr(x-x1);
	case 15:return 0;
	}
}
static void qtnear_(pqtree*q,int x,int y,int x1,int y1,int x2,int y2,uint32_t*d,pqtree**n){
	uint32_t dd=sqr(x-q->x)+sqr(y-q->y);
	if(dd<*d){
		*d=dd;
		*n=q;
	}
	int x12=x1+x2>>1,y12=y1+y2>>1;
	if(q->n[0]&&*d>pttosqdi(x,y,x12,y12,x2,y2))qtnear_(q->n[0],x,y,x12,y12,x2,y2,d,n);
	if(q->n[1]&&*d>pttosqdi(x,y,x12,y1,x2,y12))qtnear_(q->n[1],x,y,x12,y1,x2,y12,d,n);
	if(q->n[2]&&*d>pttosqdi(x,y,x1,y12,x12,y2))qtnear_(q->n[2],x,y,x1,y12,x12,y2,d,n);
	if(q->n[3]&&*d>pttosqdi(x,y,x1,y1,x12,y12))qtnear_(q->n[3],x,y,x1,y1,x12,y12,d,n);
}
pqtree*pqtnear(pqtree*q,uint16_t x,uint16_t y){
	qtnear_(q,x,y,0,0,65535,65535,(uint32_t[]){-1},&q);
	return q;
}
static void qtsnear_(pqtree*q,int x,int y,int x1,int y1,int x2,int y2,uint32_t d,qlist**n){
	if(sqr(x-q->x)+sqr(y-q->y)<d)qladd(n,q);
	int x12=x1+x2>>1,y12=y1+y2>>1;
	if(q->n[0]&&d>pttosqdi(x,y,x12,y12,x2,y2))qtsnear_(q->n[0],x,y,x12,y12,x2,y2,d,n);
	if(q->n[1]&&d>pttosqdi(x,y,x12,y1,x2,y12))qtsnear_(q->n[1],x,y,x12,y1,x2,y12,d,n);
	if(q->n[2]&&d>pttosqdi(x,y,x1,y12,x12,y2))qtsnear_(q->n[2],x,y,x1,y12,x12,y2,d,n);
	if(q->n[3]&&d>pttosqdi(x,y,x1,y1,x12,y12))qtsnear_(q->n[3],x,y,x1,y1,x12,y12,d,n);
}
qlist*pqtsnear(pqtree*q,uint16_t x,uint16_t y,uint32_t d){
	qlist*n=qlnew();
	qtsnear_(q,x,y,0,0,65535,65535,d,&n);
	return n;
}
static void qtznear_(pqtree*q,int x,int y,int x1,int y1,int x2,int y2,uint32_t*d,pqtree**n,qlist*l){
	if(!qlin(l,q)){
		uint32_t dd=sqr(x-q->x)+sqr(y-q->y);
		if(dd<*d){
			*d=dd;
			*n=q;
		}
	}
	int x12=x1+x2>>1,y12=y1+y2>>1;
	if(q->n[0]&&*d>pttosqdi(x,y,x12,y12,x2,y2))qtznear_(q->n[0],x,y,x12,y12,x2,y2,d,n,l);
	if(q->n[1]&&*d>pttosqdi(x,y,x12,y1,x2,y12))qtznear_(q->n[1],x,y,x12,y1,x2,y12,d,n,l);
	if(q->n[2]&&*d>pttosqdi(x,y,x1,y12,x12,y2))qtznear_(q->n[2],x,y,x1,y12,x12,y2,d,n,l);
	if(q->n[3]&&*d>pttosqdi(x,y,x1,y1,x12,y12))qtznear_(q->n[3],x,y,x1,y1,x12,y12,d,n,l);
}
pqtree*pqtznear(pqtree*q,uint16_t x,uint16_t y,qlist*l){
	qtznear_(q,x,y,0,0,65535,65535,(uint32_t[]){-1},&q,l);
	return q;
}
static void qtsznear_(pqtree*q,int x,int y,int x1,int y1,int x2,int y2,uint32_t d,qlist**n,qlist*l){
	if(!qlin(l,q)&&sqr(x-q->x)+sqr(y-q->y)<d)qladd(n,q);
	int x12=x1+x2>>1,y12=y1+y2>>1;
	if(q->n[0]&&d>pttosqdi(x,y,x12,y12,x2,y2))qtsznear_(q->n[0],x,y,x12,y12,x2,y2,d,n,l);
	if(q->n[1]&&d>pttosqdi(x,y,x12,y1,x2,y12))qtsznear_(q->n[1],x,y,x12,y1,x2,y12,d,n,l);
	if(q->n[2]&&d>pttosqdi(x,y,x1,y12,x12,y2))qtsznear_(q->n[2],x,y,x1,y12,x12,y2,d,n,l);
	if(q->n[3]&&d>pttosqdi(x,y,x1,y1,x12,y12))qtsznear_(q->n[3],x,y,x1,y1,x12,y12,d,n,l);
}
qlist*pqtsznear(pqtree*q,uint16_t x,uint16_t y,uint32_t d,qlist*l){
	qlist*n=qlnew();
	qtsznear_(q,x,y,0,0,65535,65535,d,&n,l);
	return n;
}