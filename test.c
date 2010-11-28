#ifdef GLX
#include <GL/glx.h>
#include <GL/glu.h>
#include <sys/unistd.h>
#include <sys/time.h>
struct timeval tvx,tvy;
#define EV(y) ev.x##y
#else
#include <SDL.h>
#include <SDL_opengl.h>
Uint32 tvx,tvy;
#define ButtonPress SDL_MOUSEBUTTONDOWN
#define MotionNotify SDL_MOUSEMOTION
#define ClientMessage SDL_QUIT
#define EV(y) ev.y
#endif
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "qtree.h"
#define case(x) break;case x:;
#define else(x) else if(x)
#include <stdio.h>
int mx,my;
void qtdraw(qtree*q,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2){
	glColor3ub(8,12,16);
	glRecti(x1,y1,x2,y2);
	glColor3ub(255,255,255);
	glBegin(GL_POINTS);
	glVertex2i(q->x,q->y);
	glEnd();
	glColor3ub(32,32,32);
	glBegin(GL_LINES);
	for(int i=0;i<4;i++)
		if(q->n[i]){
			glVertex2i(q->x,q->y);
			glVertex2i(q->n[i]->x,q->n[i]->y);
		}
	glEnd();
	uint16_t x12=x1+x2>>1,y12=y1+y2>>1;
	if(q->n[0])qtdraw(q->n[0],x12,y12,x2,y2);
	if(q->n[1])qtdraw(q->n[1],x12,y1,x2,y12);
	if(q->n[2])qtdraw(q->n[2],x1,y12,x12,y2);
	if(q->n[3])qtdraw(q->n[3],x1,y1,x12,y12);
}
int main(int argc,char**argv){
	#ifdef SDL
	if(SDL_Init(SDL_INIT_VIDEO)==-1){
		fputs(SDL_GetError(),stderr);
		return 1;
	}
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
	SDL_Surface*dpy=SDL_SetVideoMode(1024,1024,0,SDL_OPENGL);
	#else
	Display*dpy=XOpenDisplay(0);
	XVisualInfo*vi=glXChooseVisual(dpy,DefaultScreen(dpy),(int[]){GLX_RGBA,GLX_DOUBLEBUFFER,None});
	Window Wdo=XCreateWindow(dpy,RootWindow(dpy,vi->screen),0,0,1024,1024,0,vi->depth,InputOutput,vi->visual,CWColormap|CWEventMask,(XSetWindowAttributes[]){{.colormap=XCreateColormap(dpy,RootWindow(dpy,vi->screen),vi->visual,AllocNone),.event_mask=PointerMotionMask|ButtonPressMask}});
	XSetWMProtocols(dpy,Wdo,(Atom[]){XInternAtom(dpy,"WM_DELETE_WINDOW",False)},1);
	XMapWindow(dpy,Wdo);
	glXMakeCurrent(dpy,Wdo,glXCreateContext(dpy,vi,0,GL_TRUE));
	#endif
	glOrtho(0,1024,1024,0,1,-1);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE,GL_ONE);
	qtree*q=qtnew(0,0);
	for(;;){
		#ifdef GLX
		gettimeofday(&tvx,0);
		#else
		tvx=SDL_GetTicks();
		#endif
		glClear(GL_COLOR_BUFFER_BIT);
		qtdraw(q,0,0,65535,65535);
		qtree*tn=qtnear(q,mx,my);
		qlist*t=qtsnear(q,mx,my,64);
		glBegin(GL_LINES);
		glVertex2i(mx,my);
		glVertex2i(tn->x,tn->y);
		for(int i=0;i<t->n;i++){
			glVertex2i(mx,my);
			glVertex2i(t->q[i]->x,t->q[i]->y);
		}
		qldel(t);
		glEnd();
		glBegin(GL_POINTS);
		for(int i=0;i<512;i++)
			for(int j=0;j<512;j++){
				qtree*t=qtnear(q,i,j);
				glColor3ub(t->y>>4,t->x>>4,0);
				glVertex2i(i,j);
			}
		glEnd();
		#ifdef GLX
		glXSwapBuffers(dpy,Wdo);
		XEvent ev;
		while(XPending(dpy)){
			KeySym ks;
			XNextEvent(dpy,&ev);
		#else
		SDL_GL_SwapBuffers();
		SDL_Event ev;
		while(SDL_PollEvent(&ev)){
			SDLKey ks;
		#endif
			switch(ev.type){
			case(MotionNotify)
				mx=EV(button.x);
				my=EV(button.y);
			case(ButtonPress)
				switch(EV(button.button)){
				case(1)qtadd(q,EV(button.x),EV(button.y));
				case(3)
					for(int i=-6;i<7;i++)
						for(int j=-6;j<7;j++)qtsub(q,mx+i,my+j);
				}
			case(ClientMessage)return 0;
			}
		}
		int qtl=qtlen(q);
		#ifdef GLX
		gettimeofday(&tvy,0);
		#else
		tvy=SDL_GetTicks();
		#endif
		fprintf(stderr,"%d %d\n",qtl,tvy.tv_usec-tvx.tv_usec);
	}
}