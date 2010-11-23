#ifdef GLX
#include <GL/glx.h>
#include <GL/glu.h>
#include <sys/unistd.h>
#define EV(y) ev.x##y
#else
#include <SDL.h>
#include <SDL_opengl.h>
#define ButtonPress SDL_MOUSEBUTTONDOWN
#define ButtonRelease SDL_MOUSEBUTTONUP
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
int mx,my;
void qtdraw(qtree*q,uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2){
	glColor3ub(8,12,16);
	glRecti(x1,y1,x2,y2);
	glColor3ub(255,255,255);
	glBegin(GL_POINTS);
	glVertex2i(q->x,q->y);
	glEnd();
	if(q->n[0])qtdraw(q->n[0],x1+x2>>1,y1+y2>>1,x2,y2);
	if(q->n[1])qtdraw(q->n[1],x1+x2>>1,y1,x2,y1+y2>>1);
	if(q->n[2])qtdraw(q->n[2],x1,y1+y2>>1,x1+x2>>1,y2);
	if(q->n[3])qtdraw(q->n[3],x1,y1,x1+x2>>1,y1+y2>>1);
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
	Window Wdo=XCreateWindow(dpy,RootWindow(dpy,vi->screen),0,0,1024,1024,0,vi->depth,InputOutput,vi->visual,CWColormap|CWEventMask,(XSetWindowAttributes[]){{.colormap=XCreateColormap(dpy,RootWindow(dpy,vi->screen),vi->visual,AllocNone),.event_mask=ButtonPressMask}});
	XSetWMProtocols(dpy,Wdo,(Atom[]){XInternAtom(dpy,"WM_DELETE_WINDOW",False)},1);
	XMapWindow(dpy,Wdo);
	glXMakeCurrent(dpy,Wdo,glXCreateContext(dpy,vi,0,GL_TRUE));
	#endif
	srand(time(0));
	glOrtho(0,1024,1024,0,1,-1);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE,GL_ONE);
	qtree*q=qtnew(64,64);
	for(;;){
		glClear(GL_COLOR_BUFFER_BIT);
		qtdraw(q,0,0,65535,65535);
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
			case(ButtonPress)
				switch(EV(button.button)){
				case(1)qtadd(q,EV(button.x),EV(button.y));
				case(3)qtsub(q,EV(button.x),EV(button.y));
				}
			case(ClientMessage)return 0;
			}
		}
	}
}