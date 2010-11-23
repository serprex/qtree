#include "qtree.h"
static int sqr(int x){return x*x;}
static int min(int x,int y){return x>y?y:x;}
static int max(int x,int y){return x>y?x:y;}
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
unsigned qtlen(qtree*q){
	unsigned r=0;
	for(int i=0;i<4;i++)
		if(q->n[i])r+=qtlen(q->n[i]);
	return r;
}
unsigned qtdepx(qtree*q){
	unsigned r=0;
	for(int i=0;i<4;i++)
		if(q->n[i]){
			unsigned d=qtdepx(q->n[i]);
			r=max(r,d);
		}
	return r;
}
unsigned qtdepn(qtree*q){
	unsigned r=0;
	for(int i=0;i<4;i++)
		if(q->n[i]){
			unsigned d=qtdepn(q->n[i]);
			r=min(r,d);
		}
	return r;
}
static qtree*qtadd_(qtree*q,uint16_t x,uint16_t y,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2){
	if(x==q->x&&y==q->y)return q;
	else if(x>x1+x2>>1){
		if(y>y1+y2>>1){
			if(!q->n[0]){
				(q->n[0]=qtnew(x,y))->n[4]=q;
				return q->n[0];
			}else return qtadd_(q->n[0],x,y,x1+x2>>1,y1+y2>>1,x2,y2);
		}else{
			if(!q->n[1]){
				(q->n[1]=qtnew(x,y))->n[4]=q;
				return q->n[1];
			}else return qtadd_(q->n[1],x,y,x1+x2>>1,y1,x2,y1+y2>>1);
		}
	}else{
		if(y>y1+y2>>1){
			if(!q->n[2]){
				(q->n[2]=qtnew(x,y))->n[4]=q;
				return q->n[2];
			}else qtadd_(q->n[2],x,y,x1,y1+y2>>1,x1+x2>>1,y2);
		}else{
			if(!q->n[3]){
				(q->n[3]=qtnew(x,y))->n[4]=q;
				return q->n[3];
			}else qtadd_(q->n[3],x,y,x1,y1,x1+x2>>1,y1+y2>>1);
		}
	}
}
qtree*qtadd(qtree*q,uint16_t x,uint16_t y){
	return qtadd_(q,x,y,0,0,65535,65535);
}
static qtree*qtget_(qtree*q,uint16_t x,uint16_t y,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2){
	if(x==q->x&&y==q->y)return q;
	else if(x>x1+x2>>1){
		if(y>y1+y2>>1)return q->n[0]?qtget_(q->n[0],x,y,x1+x2>>1,y1+y2>>1,x2,y2):0;
		else return q->n[1]?qtget_(q->n[1],x,y,x1+x2>>1,y1,x2,y1+y2>>1):0;
	}else if(y>y1+y2>>1)return q->n[2]?qtget_(q->n[2],x,y,x1,y1+y2>>1,x1+x2>>1,y2):0;
	else return q->n[3]?qtget_(q->n[3],x,y,x1,y1,x1+x2>>1,y1+y2>>1):0;
}
qtree*qtget(qtree*q,uint16_t x,uint16_t y){
	return qtget_(q,x,y,0,0,65535,65535);
}
void qtsubdir(qtree*q){
	for(int i=0;i<4;i++)
		if(q->n[i]){
			q->x=q->n[i]->x;
			q->y=q->n[i]->y;
			qtsubdir(q->n[i]);
			return;
		}
	for(int i=0;i<4;i++)
		if(q->n[4]->n[i]==q){
			q->n[4]->n[i]=0;
			free(q);
			return;
		}
	__builtin_unreachable();
}
void qtsub(qtree*q,uint16_t x,uint16_t y){
	if(q=qtget(q,x,y))qtsubdir(q);
}
qtree*qtcpy(qtree*q){
	qtree*n=qtnew(q->x,q->y);
	n->d=q->d;
	for(int i=0;i<4;i++)
		if(q->n[i])n->n[i]=qtcpy(q->n[i]);
	return n;
}
static int pttosqrdis(uint16_t x,uint16_t y,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2){
	return x>=x1&&x<=x2&&y>=y1&&y<=y2?0:
	x>=x1&&x<=x2?sqr(min(abs(y-y1),abs(y-y2))):
	y>=y1&&y<=y2?sqr(min(abs(x-x1),abs(x-x2))):
	x>=x2&&y>=y2?sqr(x-x2)+sqr(y-y2):
	x<=x1&&y<=y1?sqr(x-x1)+sqr(y-y1):
	x<=x1&&y>=y2?sqr(x-x1)+sqr(y-y2):
	sqr(x-x2)+sqr(y-y1);
}
static void qtnearxy_(qtree*q,uint16_t x,uint16_t y,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2,int*d,qtree**n){
	int dd=sqr(x-q->x)+sqr(y-q->y);
	if(dd<*d){
		*d=dd;
		*n=q;
	}
	if(q->n[0]&&*d>pttosqrdis(x,y,x1+x2>>1,y1+y2>>1,x2,y2))qtnearxy_(q->n[0],x,y,x1+x2>>1,y1+y2>>1,x2,y2,d,n);
	if(q->n[1]&&*d>pttosqrdis(x,y,x1+x2>>1,y1,x2,y1+y2>>1))qtnearxy_(q->n[1],x,y,x1+x2>>1,y1,x2,y1+y2>>1,d,n);
	if(q->n[2]&&*d>pttosqrdis(x,y,x1,y1+y2>>1,x1+x2>>1,y2))qtnearxy_(q->n[2],x,y,x1,y1+y2>>1,x1+x2>>1,y2,d,n);
	if(q->n[3]&&*d>pttosqrdis(x,y,x1,y1,x1+x2>>1,y1+y2>>1))qtnearxy_(q->n[3],x,y,x1,y1,x1+x2>>1,y1+y2>>1,d,n);
}
qtree*qtnearxy(qtree*q,uint16_t x,uint16_t y){
	int d=sqr(x-q->x)+sqr(y-q->y);
	qtnearxy_(q,x,y,0,0,65535,65535,&d,&q);
	return q;
}