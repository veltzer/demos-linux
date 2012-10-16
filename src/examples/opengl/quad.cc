/*
	This file is part of the linuxapi project.
	Copyright (C) 2011, 2012 Mark Veltzer <mark.veltzer@gmail.com>

	The linuxapi package is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	The linuxapi package is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with the GNU C Library; if not, write to the Free
	Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
	02111-1307 USA.
*/

#include<firstinclude.h>
#include<stdio.h>
#include<stdlib.h>
#include<X11/X.h>
#include<X11/Xlib.h>
#include<GL/gl.h>
#include<GL/glx.h>
#include<GL/glu.h>

/*
* OpenGL example number 1...
* Taken from http://www.opengl.org/wiki/Programming_OpenGL_in_Linux:_GLX_and_Xlib
*
* EXTRA_LIBS=-lX11 -lGL -lGLU
*/

Display* dpy;
Window root;
GLint att[]={ GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
XVisualInfo* vi;
Colormap cmap;
XSetWindowAttributes swa;
Window win;
GLXContext glc;
XWindowAttributes gwa;
XEvent xev;

void DrawAQuad() {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1., 1., -1., 1., 1., 20.);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0., 0., 10., 0., 0., 0., 0., 1., 0.);

	glBegin(GL_QUADS);
	glColor3f(1., 0., 0.); glVertex3f(-.75,-.75,0.);
	glColor3f(0., 1., 0.); glVertex3f(+.75,-.75,0.);
	glColor3f(0., 0., 1.); glVertex3f(+.75,+.75,0.);
	glColor3f(1., 1., 0.); glVertex3f(-.75,+.75,0.);
	glEnd();
}

int main(int argc,char** argv,char** envp) {
	dpy=XOpenDisplay(NULL);
	if(dpy==NULL) {
		printf("cannot connect to X server\n");
		exit(0);
	}
	root=DefaultRootWindow(dpy);
	vi=glXChooseVisual(dpy, 0, att);
	if(vi==NULL) {
		printf("no appropriate visual found\n");
		exit(0);
	}
	//printf("tvisual %p selected\n", (void *)vi->visualid); }/* %p creates hexadecimal output like in glxinfo */
	cmap=XCreateColormap(dpy, root, vi->visual, AllocNone);
	swa.colormap=cmap;
	swa.event_mask=ExposureMask | KeyPressMask;
	win=XCreateWindow(dpy, root, 0, 0, 600, 600, 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
	XMapWindow(dpy, win);
	XStoreName(dpy, win, "VERY SIMPLE APPLICATION");
	glc=glXCreateContext(dpy, vi, NULL, GL_TRUE);
	glXMakeCurrent(dpy, win, glc);
	glEnable(GL_DEPTH_TEST);
	while(true) {
		XNextEvent(dpy, &xev);
		if(xev.type==Expose) {
			printf("got expose\n");
			XGetWindowAttributes(dpy, win, &gwa);
			glViewport(0, 0, gwa.width, gwa.height);
			DrawAQuad();
			glXSwapBuffers(dpy, win);
		} else if(xev.type==KeyPress) {
			printf("got key press\n");
			glXMakeCurrent(dpy, None, NULL);
			glXDestroyContext(dpy, glc);
			XDestroyWindow(dpy, win);
			XCloseDisplay(dpy);
			exit(0);
		}
	}
	return EXIT_SUCCESS;
}
