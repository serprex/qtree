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
#include "rqtree.h"
#define case(x) break;case x:;
#define else(x) else if(x)
#include <stdio.h>
int mx,my,mxo,myo,mz;
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
	rqtree*q=rqtnew();
	for(;;){
		#ifdef GLX
		gettimeofday(&tvx,0);
		#else
		tvx=SDL_GetTicks();
		#endif
		glClear(GL_COLOR_BUFFER_BIT);
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
				case(1)
					if(mz=!mz){
						mxo=mx;
						myo=my;
					}else rqtadd(q,mx,my,mxo,myo);
				case(3)rqtsubp(q,mx,my);
				}
			case(ClientMessage)return 0;
			}
		}
		int qtl=rqtlen(q);
		#ifdef GLX
		gettimeofday(&tvy,0);
		fprintf(stderr,"%d %d %d\n",qtl,tvy.tv_usec-tvx.tv_usec,tvy.tv_sec-tvx.tv_sec);
		#else
		tvy=SDL_GetTicks();
		#endif
	}
}