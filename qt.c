#include "qtree.h"
#include <stdlib.h>
static int sqr(int x){return x*x;}
static int min(int x,int y){return x>y?y:x;}
static int max(int x,int y){return x>y?x:y;}
qlist*qlnew(qtree*q){
	qlist*r=calloc(1,sizeof(qlist));
	r->q=q;
	return r;
}
qlist*qlnewx(qtree**x){
	qlist*r=qlnew(*x);
	while(*++x)qladd(&r,*x);
	return r;
}
void qldel(qlist*n){
	if(n->n)qldel(n->n);
	free(n);
}
void qladd(qlist**n,qtree*q){
	qlist*l=qlnew(q);
	l->n=*n;
	*n=l;
}
int qlin(qlist*n,qtree*q){
	return n->q==q||(n->n?qlin(n->n,q):0);
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
static qtree*qtadd_(qtree*q,uint16_t x,uint16_t y,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2){
	if(x==q->x&&y==q->y)return q;
	uint16_t x12=x1+x2>>1,y12=y1+y2>>1;
	if(x>x12){
		if(y>y12){
			if(!q->n[0])return q->n[0]=qtnew(x,y);
			else return qtadd_(q->n[0],x,y,x12,y12,x2,y2);
		}else if(!q->n[1])return q->n[1]=qtnew(x,y);
		else return qtadd_(q->n[1],x,y,x12,y1,x2,y12);
	}else if(y>y12){
		if(!q->n[2])return q->n[2]=qtnew(x,y);
		else return qtadd_(q->n[2],x,y,x1,y12,x12,y2);
	}else if(!q->n[3])return q->n[3]=qtnew(x,y);
	else return qtadd_(q->n[3],x,y,x1,y1,x12,y12);
}
qtree*qtadd(qtree*q,uint16_t x,uint16_t y){return qtadd_(q,x,y,0,0,65535,65535);}
static qtree*qtget_(qtree*q,uint16_t x,uint16_t y,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2){
	if(x==q->x&&y==q->y)return q;
	uint16_t x12=x1+x2>>1,y12=y1+y2>>1;
	if(x>x12){
		if(y>y12)return q->n[0]?qtget_(q->n[0],x,y,x12,y12,x2,y2):0;
		else return q->n[1]?qtget_(q->n[1],x,y,x12,y1,x2,y12):0;
	}else if(y>y12)return q->n[2]?qtget_(q->n[2],x,y,x1,y12,x12,y2):0;
	else return q->n[3]?qtget_(q->n[3],x,y,x1,y1,x12,y12):0;
}
qtree*qtget(qtree*q,uint16_t x,uint16_t y){return qtget_(q,x,y,0,0,65535,65535);}
static void qtsubdir(qtree*q,qtree*h){
	for(int i=0;i<4;i++)
		if(q->n[i]){
			q->x=q->n[i]->x;
			q->y=q->n[i]->y;
			qtsubdir(q->n[i],q);
			return;
		}
	for(int i=0;i<4;i++)
		if(h->n[i]==q){
			h->n[i]=0;
			free(q);
			return;
		}
	__builtin_unreachable();
}
static void qtsub_(qtree*q,uint16_t x,uint16_t y,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,qtree*h){
	if(x==q->x&&y==q->y)qtsubdir(q,h);
	else{
		uint16_t x12=x1+x2>>1,y12=y1+y2>>1;
		if(x>x12){
			if(y>y12){
				if(q->n[0])qtsub_(q->n[0],x,y,x12,y12,x2,y2,q);
			}else if(q->n[1])qtsub_(q->n[1],x,y,x12,y1,x2,y12,q);
		}else if(y>y12){
			if(q->n[2])qtsub_(q->n[2],x,y,x1,y12,x12,y2,q);
		}else if(q->n[3])qtsub_(q->n[3],x,y,x1,y1,x12,y12,q);
	}
}
void qtsub(qtree*q,uint16_t x,uint16_t y){if(x!=q->x||y!=q->y)qtsub_(q,x,y,0,0,65535,65535,0);}
qtree*qtcpy(qtree*q){
	qtree*n=qtnew(q->x,q->y);
	for(int i=0;i<4;i++)
		if(q->n[i])n->n[i]=qtcpy(q->n[i]);
	return n;
}
static int pttosqdi(uint16_t x,uint16_t y,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2){
	return x>=x1&&x<=x2&&y>=y1&&y<=y2?0:
	x>=x1&&x<=x2?sqr(min(abs(y-y1),abs(y-y2))):
	y>=y1&&y<=y2?sqr(min(abs(x-x1),abs(x-x2))):
	x>=x2&&y>=y2?sqr(x-x2)+sqr(y-y2):
	x<=x1&&y<=y1?sqr(x-x1)+sqr(y-y1):
	x<=x1&&y>=y2?sqr(x-x1)+sqr(y-y2):
	sqr(x-x2)+sqr(y-y1);
}
static void qtnear_(qtree*q,uint16_t x,uint16_t y,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint32_t*d,qtree**n){
	uint32_t dd=sqr(x-q->x)+sqr(y-q->y);
	if(dd<*d){
		*d=dd;
		*n=q;
	}
	uint16_t x12=x1+x2>>1,y12=y1+y2>>1;
	if(q->n[0]&&*d>pttosqdi(x,y,x12,y12,x2,y2))qtnear_(q->n[0],x,y,x12,y12,x2,y2,d,n);
	if(q->n[1]&&*d>pttosqdi(x,y,x12,y1,x2,y12))qtnear_(q->n[1],x,y,x12,y1,x2,y12,d,n);
	if(q->n[2]&&*d>pttosqdi(x,y,x1,y12,x12,y2))qtnear_(q->n[2],x,y,x1,y12,x12,y2,d,n);
	if(q->n[3]&&*d>pttosqdi(x,y,x1,y1,x12,y12))qtnear_(q->n[3],x,y,x1,y1,x12,y12,d,n);
}
qtree*qtnear(qtree*q,uint16_t x,uint16_t y){
	qtnear_(q,x,y,0,0,65535,65535,(uint32_t[]){-1},&q);
	return q;
}
static void qtsnear_(qtree*q,uint16_t x,uint16_t y,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint32_t d,qlist**n){
	uint32_t dd=sqr(x-q->x)+sqr(y-q->y);
	if(dd<d)qladd(n,q);
	uint16_t x12=x1+x2>>1,y12=y1+y2>>1;
	if(q->n[0]&&d>pttosqdi(x,y,x12,y12,x2,y2))qtsnear_(q->n[0],x,y,x12,y12,x2,y2,d,n);
	if(q->n[1]&&d>pttosqdi(x,y,x12,y1,x2,y12))qtsnear_(q->n[1],x,y,x12,y1,x2,y12,d,n);
	if(q->n[2]&&d>pttosqdi(x,y,x1,y12,x12,y2))qtsnear_(q->n[2],x,y,x1,y12,x12,y2,d,n);
	if(q->n[3]&&d>pttosqdi(x,y,x1,y1,x12,y12))qtsnear_(q->n[3],x,y,x1,y1,x12,y12,d,n);
}
qlist*qtsnear(qtree*q,uint16_t x,uint16_t y,uint32_t d){
	qlist*n=0;
	qtsnear_(q,x,y,0,0,65535,65535,d*d,&n);
	return n;
}
static void qtznear_(qtree*q,uint16_t x,uint16_t y,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint32_t*d,qtree**n,qlist*l){
	if(!qlin(l,q)){
		uint32_t dd=sqr(x-q->x)+sqr(y-q->y);
		if(dd<*d){
			*d=dd;
			*n=q;
		}
	}
	uint16_t x12=x1+x2>>1,y12=y1+y2>>1;
	if(q->n[0]&&*d>pttosqdi(x,y,x12,y12,x2,y2))qtznear_(q->n[0],x,y,x12,y12,x2,y2,d,n,l);
	if(q->n[1]&&*d>pttosqdi(x,y,x12,y1,x2,y12))qtznear_(q->n[1],x,y,x12,y1,x2,y12,d,n,l);
	if(q->n[2]&&*d>pttosqdi(x,y,x1,y12,x12,y2))qtznear_(q->n[2],x,y,x1,y12,x12,y2,d,n,l);
	if(q->n[3]&&*d>pttosqdi(x,y,x1,y1,x12,y12))qtznear_(q->n[3],x,y,x1,y1,x12,y12,d,n,l);
}
qtree*qtznear(qtree*q,uint16_t x,uint16_t y,qlist*l){
	qtznear_(q,x,y,0,0,65535,65535,(uint32_t[]){-1},&q,l);
	return q;
}
static void qtsznear_(qtree*q,uint16_t x,uint16_t y,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,uint32_t d,qlist**n,qlist*l){
	if(!qlin(l,q)){
		uint32_t dd=sqr(x-q->x)+sqr(y-q->y);
		if(dd<d)qladd(n,q);
	}
	uint16_t x12=x1+x2>>1,y12=y1+y2>>1;
	if(q->n[0]&&d>pttosqdi(x,y,x12,y12,x2,y2))qtsznear_(q->n[0],x,y,x12,y12,x2,y2,d,n,l);
	if(q->n[1]&&d>pttosqdi(x,y,x12,y1,x2,y12))qtsznear_(q->n[1],x,y,x12,y1,x2,y12,d,n,l);
	if(q->n[2]&&d>pttosqdi(x,y,x1,y12,x12,y2))qtsznear_(q->n[2],x,y,x1,y12,x12,y2,d,n,l);
	if(q->n[3]&&d>pttosqdi(x,y,x1,y1,x12,y12))qtsznear_(q->n[3],x,y,x1,y1,x12,y12,d,n,l);
}
qlist*qtsznear(qtree*q,uint16_t x,uint16_t y,uint32_t d,qlist*l){
	qlist*n=0;
	qtsznear_(q,x,y,0,0,65535,65535,d*d,&n,l);
	return n;
}