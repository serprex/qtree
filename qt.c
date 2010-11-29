#include "qtree.h"
#include <stdlib.h>
static uint32_t sqr(uint32_t x){return x*x;}
qlist*qlnew(){return calloc(1,sizeof(qlist));}
void qldel(qlist*n){free(n);}
void qladd(qlist**n,qtree*q){
	*n=realloc(*n,sizeof(qlist)+sizeof(qtree*)*++(*n)->n);
	(*n)->q[(*n)->n-1]=q;
}
int qlin(qlist*n,qtree*q){
	for(uint32_t i=0;i<n->n;i++)
		if(n->q[i]==q)return 1;
	return 0;
}
void qlset(qlist*n,uint32_t i){if(i<n->n)n->p=i;}
qtree*qlnxt(qlist*n){return n->p<n->n?n->q[n->p++]:0;}
static void qlqt_(qtree*q,qlist*n){
	n->q[n->p++]=q;
	for(int i=0;i<4;i++)
		if(q->n[i])qlqt_(q->n[i],n);
}
qlist*qlqt(qtree*q){
	uint32_t ql=qtlen(q);
	qlist*n=malloc(sizeof(qlist)+sizeof(qtree*)*ql);
	n->n=ql;
	qlqt_(q,n);
	n->p=0;
	return n;
}
qtree*qtnew(uint16_t x,uint16_t y){
	qtree*r=calloc(1,sizeof(qtree));
	r->x=x;
	r->y=y;
	return r;
}
void qtdel(qtree*q){
	for(int i=0;i<4;i++)
		if(q->n[i])qtdel(q->n[i]);
	free(q);
}
uint32_t qtlen(qtree*q){
	uint32_t r=1;
	for(int i=0;i<4;i++)
		if(q->n[i])r+=qtlen(q->n[i]);
	return r;
}
static qtree*qtadd_(qtree*q,int x,int y,int x1,int y1,int x2,int y2){
	if(x==q->x&&y==q->y)return q;
	int x12=x1+x2>>1,y12=y1+y2>>1;
	if(x>x12){
		if(y>y12){
			if(q->n[0])return qtadd_(q->n[0],x,y,x12,y12,x2,y2);
			else return q->n[0]=qtnew(x,y);
		}else if(q->n[1])return qtadd_(q->n[1],x,y,x12,y1,x2,y12);
		else return q->n[1]=qtnew(x,y);
	}else if(y>y12){
		if(q->n[2])return qtadd_(q->n[2],x,y,x1,y12,x12,y2);
		else return q->n[2]=qtnew(x,y);
	}else if(q->n[3])return qtadd_(q->n[3],x,y,x1,y1,x12,y12);
	else return q->n[3]=qtnew(x,y);
}
qtree*qtadd(qtree*q,uint16_t x,uint16_t y){return qtadd_(q,x,y,0,0,65535,65535);}
static qtree*qtget_(qtree*q,int x,int y,int x1,int y1,int x2,int y2){
	if(x==q->x&&y==q->y)return q;
	int x12=x1+x2>>1,y12=y1+y2>>1;
	if(x>x12){
		if(y>y12)return q->n[0]?qtget_(q->n[0],x,y,x12,y12,x2,y2):0;
		else return q->n[1]?qtget_(q->n[1],x,y,x12,y1,x2,y12):0;
	}else if(y>y12)return q->n[2]?qtget_(q->n[2],x,y,x1,y12,x12,y2):0;
	else return q->n[3]?qtget_(q->n[3],x,y,x1,y1,x12,y12):0;
}
qtree*qtget(qtree*q,uint16_t x,uint16_t y){return qtget_(q,x,y,0,0,65535,65535);}
static void qtsub__(qtree*q,qtree**h){
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
static void qtsub_(qtree*q,int x,int y,int x1,int y1,int x2,int y2,qtree**h){
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
void qtsub(qtree*q,uint16_t x,uint16_t y){qtsub_(q,x,y,0,0,65535,65535,0);}
qtree*qtcpy(qtree*q){
	qtree*n=qtnew(q->x,q->y);
	for(int i=0;i<4;i++)
		if(q->n[i])n->n[i]=qtcpy(q->n[i]);
	return n;
}
static uint32_t pttosqdi(int x,int y,int x1,int y1,int x2,int y2){
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
static void qtnear_(qtree*q,int x,int y,int x1,int y1,int x2,int y2,uint32_t*d,qtree**n){
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
qtree*qtnear(qtree*q,uint16_t x,uint16_t y){
	qtnear_(q,x,y,0,0,65535,65535,(uint32_t[]){-1},&q);
	return q;
}
static void qtsnear_(qtree*q,int x,int y,int x1,int y1,int x2,int y2,uint32_t d,qlist**n){
	if(sqr(x-q->x)+sqr(y-q->y)<d)qladd(n,q);
	int x12=x1+x2>>1,y12=y1+y2>>1;
	if(q->n[0]&&d>pttosqdi(x,y,x12,y12,x2,y2))qtsnear_(q->n[0],x,y,x12,y12,x2,y2,d,n);
	if(q->n[1]&&d>pttosqdi(x,y,x12,y1,x2,y12))qtsnear_(q->n[1],x,y,x12,y1,x2,y12,d,n);
	if(q->n[2]&&d>pttosqdi(x,y,x1,y12,x12,y2))qtsnear_(q->n[2],x,y,x1,y12,x12,y2,d,n);
	if(q->n[3]&&d>pttosqdi(x,y,x1,y1,x12,y12))qtsnear_(q->n[3],x,y,x1,y1,x12,y12,d,n);
}
qlist*qtsnear(qtree*q,uint16_t x,uint16_t y,uint32_t d){
	qlist*n=qlnew();
	qtsnear_(q,x,y,0,0,65535,65535,d*d,&n);
	return n;
}
static void qtznear_(qtree*q,int x,int y,int x1,int y1,int x2,int y2,uint32_t*d,qtree**n,qlist*l){
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
qtree*qtznear(qtree*q,uint16_t x,uint16_t y,qlist*l){
	qtznear_(q,x,y,0,0,65535,65535,(uint32_t[]){-1},&q,l);
	return q;
}
static void qtsznear_(qtree*q,int x,int y,int x1,int y1,int x2,int y2,uint32_t d,qlist**n,qlist*l){
	if(!qlin(l,q)&&sqr(x-q->x)+sqr(y-q->y)<d)qladd(n,q);
	int x12=x1+x2>>1,y12=y1+y2>>1;
	if(q->n[0]&&d>pttosqdi(x,y,x12,y12,x2,y2))qtsznear_(q->n[0],x,y,x12,y12,x2,y2,d,n,l);
	if(q->n[1]&&d>pttosqdi(x,y,x12,y1,x2,y12))qtsznear_(q->n[1],x,y,x12,y1,x2,y12,d,n,l);
	if(q->n[2]&&d>pttosqdi(x,y,x1,y12,x12,y2))qtsznear_(q->n[2],x,y,x1,y12,x12,y2,d,n,l);
	if(q->n[3]&&d>pttosqdi(x,y,x1,y1,x12,y12))qtsznear_(q->n[3],x,y,x1,y1,x12,y12,d,n,l);
}
qlist*qtsznear(qtree*q,uint16_t x,uint16_t y,uint32_t d,qlist*l){
	qlist*n=qlnew();
	qtsznear_(q,x,y,0,0,65535,65535,d*d,&n,l);
	return n;
}