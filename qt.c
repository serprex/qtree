#include "qtree.h"
#define qsz sizeof(qtree)
qtree*qtnew(uint16_t x,uint16_t y){
	qtree*r=calloc(1,qsz);
	r->x=x;
	r->y=y;
	return r;
}
void qtdel(qtree*q){
	for(int i=0;i<4;i++)
		if(q->n[i])qtdel(q->n[i]);
	free(q);
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
static void qtsub_(qtree*q){
	for(int i=0;i<4;i++)
		if(q->n[i]){
			q->x=q->n[i]->x;
			q->y=q->n[i]->y;
			qtsub_(q->n[i]);
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
	if(q=qtget(q,x,y))qtsub_(q);
}
qtree*qtcpy(qtree*q){
	qtree*n=qtnew(q->x,q->y);
	for(int i=0;i<4;i++)
		if(q->n[i])n->n[i]=qtcpy(q->n[i]);
	return n;
}