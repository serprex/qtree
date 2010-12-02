#include <stdlib.h>
#include "qlist.h"
qlist*qlnew(){return calloc(1,sizeof(qlist));}
void qldel(qlist*n){free(n);}
void qladd(qlist**n,void*q){
	*n=realloc(*n,sizeof(qlist)+sizeof(void*)*++(*n)->n);
	(*n)->q[(*n)->n-1]=q;
}
int qlin(qlist*n,void*q){
	for(uint32_t i=0;i<n->n;i++)
		if(n->q[i]==q)return 1;
	return 0;
}
void qlset(qlist*n,uint32_t i){if(i<n->n)n->p=i;}
void*qlnxt(qlist*n){return n->p<n->n?n->q[n->p++]:0;}