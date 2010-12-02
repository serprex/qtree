#pragma once
#include <stdint.h>
typedef struct qlist{
	uint32_t n;
	uint32_t p;
	void*q[];
}qlist;
typedef struct pqtree{
	struct pqtree*n[4];
	uint16_t x,y;
}pqtree;
typedef struct rqtree{
	struct rqtree*n[4];
	uint16_t x;
	uint16_t*r;
}rqtree;