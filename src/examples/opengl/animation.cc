#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<math.h>
#include<time.h>
#include<sys/time.h>
#include<X11/Xlib.h>
#include<GL/glx.h>
#include<GL/glext.h>
#include<GL/glu.h>

/*
 * OpenGL example number 2...
 * Taken from http://www.opengl.org/wiki/Programming_OpenGL_in_Linux:_Programming_Animations_with_GLX_and_Xlib
 *
 *			Mark Veltzer
 *
 * EXTRA_LIBS=-lX11 -lGL -lGLU
 */
//////////////////////////////////////////////////////////////////////////////////
//				GLOBAL IDENTIFIERS				//
//////////////////////////////////////////////////////////////////////////////////
Display* dpy;
Window root, win;
GLint att[]={ GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
XVisualInfo* vi;
GLXContext glc;
Colormap cmap;
XSetWindowAttributes swa;
XWindowAttributes	wa;
XEvent			xev;

float			TimeCounter, LastFrameTimeCounter, DT, prevTime = 0.0, FPS;
struct timeval		tv, tv0;
int			Frame = 1, FramesPerFPS;

GLfloat			rotation_matrix[16];
float			rot_z_vel = 50.0, rot_y_vel = 30.0;
//////////////////////////////////////////////////////////////////////////////////
//				DRAW A CUBE					//
//////////////////////////////////////////////////////////////////////////////////
void DrawCube(float size) {
	glBegin(GL_QUADS);

	glColor3f(0.7, 0.0, 0.0);
	glVertex3f(-size,-size,-size);
	glVertex3f(+size,-size,-size);
	glVertex3f(+size,+size,-size);
	glVertex3f(-size,+size,-size);

	glVertex3f(-size,-size,+size);
	glVertex3f(+size,-size,+size);
	glVertex3f(+size,+size,+size);
	glVertex3f(-size,+size,+size);

	glColor3f(0.0, 0.0, 0.7);

	glVertex3f(-size,-size,-size);
	glVertex3f(-size,-size,+size);
	glVertex3f(-size,+size,+size);
	glVertex3f(-size,+size,-size);

	glVertex3f(+size,-size,-size);
	glVertex3f(+size,-size,+size);
	glVertex3f(+size,+size,+size);
	glVertex3f(+size,+size,-size);

	glColor3f(0.0, 0.7, 0.0);

	glVertex3f(-size,-size,-size);
	glVertex3f(-size,-size,+size);
	glVertex3f(+size,-size,+size);
	glVertex3f(+size,-size,-size);

	glVertex3f(-size,+size,-size);
	glVertex3f(-size,+size,+size);
	glVertex3f(+size,+size,+size);
	glVertex3f(+size,+size,-size);

	glEnd();
}
//////////////////////////////////////////////////////////////////////////////////
//				ROTATE THE CUBE					//
//////////////////////////////////////////////////////////////////////////////////
void RotateCube() {
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(rot_y_vel*DT, 0.0, 1.0, 0.0);
	glRotatef(rot_z_vel*DT, 0.0, 0.0, 1.0);
	glMultMatrixf(rotation_matrix);
	glGetFloatv(GL_MODELVIEW_MATRIX, rotation_matrix);
}
//////////////////////////////////////////////////////////////////////////////////
//				EXPOSURE FUNCTION				//
//////////////////////////////////////////////////////////////////////////////////
void ExposeFunc() {
 float	aspect_ratio;
 char	info_string[256];
 /////////////////////////////////
 //	RESIZE VIEWPORT		//
 /////////////////////////////////
 XGetWindowAttributes(dpy, win, &wa);
 glViewport(0, 0, wa.width, wa.height);
 aspect_ratio = (float)(wa.width) / (float)(wa.height);
 /////////////////////////////////////////
 //	SETUP PROJECTION & MODELVIEW	//
 /////////////////////////////////////////
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 glOrtho(-2.50*aspect_ratio, 2.50*aspect_ratio, -2.50, 2.50, 1., 100.);

 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
 gluLookAt(10., 0., 0., 0., 0., 0., 0., 0., 1.);
 glMultMatrixf(rotation_matrix);
 /////////////////////////////////
 //	DRAW CUBE		//
 /////////////////////////////////
 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 DrawCube(1.0);
 /////////////////////////////////
 //	DISPLAY TIME, FPS etc.	//
 /////////////////////////////////
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 glOrtho(0, (float)wa.width, 0, (float)wa.height, -1., 1.);

 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();

 glColor3f(1.0, 1.0, 1.0);

 sprintf(info_string, "%4.1f seconds * %4.1f fps at %i x %i", TimeCounter, FPS, wa.width, wa.height);
 glRasterPos2i(10, 10);
 glCallLists(strlen(info_string), GL_UNSIGNED_BYTE, info_string);

 sprintf(info_string, "<up,down,left,right> rotate cube * <F1> stop rotation ");
 glRasterPos2i(10, wa.height-32);
 glCallLists(strlen(info_string), GL_UNSIGNED_BYTE, info_string);
 /////////////////////////////////
 //	SWAP BUFFERS		//
 /////////////////////////////////
 glXSwapBuffers(dpy, win); }
//////////////////////////////////////////////////////////////////////////////////
//				CREATE A GL CAPABLE WINDOW			//
//////////////////////////////////////////////////////////////////////////////////
void CreateWindow() {

 if((dpy = XOpenDisplay(NULL)) == NULL) {
 	printf("\n\tcannot connect to x server\n\n");
	exit(0); }

 root = DefaultRootWindow(dpy);

 if((vi = glXChooseVisual(dpy, 0, att)) == NULL) {
	printf("\n\tno matching visual\n\n");
	exit(0); }
	
 if((cmap = XCreateColormap(dpy, root, vi->visual, AllocNone)) == 0) {
 	printf("\n\tcannot create colormap\n\n");
	exit(0); }
	
 swa.event_mask = KeyPressMask;
 swa.colormap 	= cmap;
 win = XCreateWindow(dpy, root, 0, 0, 700, 700, 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);
 XStoreName(dpy, win, "OpenGL Animation");
 XMapWindow(dpy, win); }
//////////////////////////////////////////////////////////////////////////////////
//				SETUP GL CONTEXT				//
//////////////////////////////////////////////////////////////////////////////////
void SetupGL() {
 char		font_string[128];
 XFontStruct	*font_struct;
 /////////////////////////////////////////////////
 //	CREATE GL CONTEXT AND MAKE IT CURRENT	//
 /////////////////////////////////////////////////
 if((glc = glXCreateContext(dpy, vi, NULL, GL_TRUE)) == NULL) {
 	printf("\n\tcannot create gl context\n\n");
	exit(0); }

 glXMakeCurrent(dpy, win, glc);
 glEnable(GL_DEPTH_TEST);
 glClearColor(0.00, 0.00, 0.40, 1.00);
 /////////////////////////////////////////////////
 //	FIND A FONT				//
 /////////////////////////////////////////////////
 for(int font_size = 14; font_size < 32; font_size += 2) {
 	sprintf(font_string, "-adobe-courier-*-r-normal--%i-*", font_size);
	font_struct = XLoadQueryFont(dpy, font_string);
	
	if(font_struct != NULL) {
 		glXUseXFont(font_struct->fid, 32, 192, 32);		
		break; } }
 /////////////////////////////////////////////////
 //	INITIALIZE ROTATION MATRIX		//
 /////////////////////////////////////////////////
 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
 glGetFloatv(GL_MODELVIEW_MATRIX, rotation_matrix); }
//////////////////////////////////////////////////////////////////////////////////
//				TIME COUNTER FUNCTIONS				//
//////////////////////////////////////////////////////////////////////////////////
void InitTimeCounter() {
 gettimeofday(&tv0, NULL);
 FramesPerFPS = 30; }

void UpdateTimeCounter() {
 LastFrameTimeCounter = TimeCounter;
 gettimeofday(&tv, NULL);
 TimeCounter = (float)(tv.tv_sec-tv0.tv_sec) + 0.000001*((float)(tv.tv_usec-tv0.tv_usec));
 DT = TimeCounter - LastFrameTimeCounter; }

void CalculateFPS() {
 Frame ++;

 if((Frame%FramesPerFPS) == 0) {
	FPS = ((float)(FramesPerFPS)) / (TimeCounter-prevTime);
	prevTime = TimeCounter; } }
//////////////////////////////////////////////////////////////////////////////////
//				EXIT PROGRAM					//
//////////////////////////////////////////////////////////////////////////////////
void ExitProgram() {
 glXMakeCurrent(dpy, None, NULL);
 glXDestroyContext(dpy, glc);
 XDestroyWindow(dpy, win);
 XCloseDisplay(dpy);
 exit(0); }
//////////////////////////////////////////////////////////////////////////////////
//				CHECK EVENTS					//
//////////////////////////////////////////////////////////////////////////////////
void CheckKeyboard() {

 if(XCheckWindowEvent(dpy, win, KeyPressMask, &xev)) {
	char	*key_string = XKeysymToString(XKeycodeToKeysym(dpy, xev.xkey.keycode, 0));

	if(strncmp(key_string, "Left", 4) == 0) {
		rot_z_vel -= 200.0*DT; }

	else if(strncmp(key_string, "Right", 5) == 0) {
		rot_z_vel += 200.0*DT; }

	else if(strncmp(key_string, "Up", 2) == 0) {
		rot_y_vel -= 200.0*DT; }

	else if(strncmp(key_string, "Down", 4) == 0) {
		rot_y_vel += 200.0*DT; }

	else if(strncmp(key_string, "F1", 2) == 0) {
		rot_y_vel = 0.0;
		rot_z_vel = 0.0; }

	else if(strncmp(key_string, "Escape", 5) == 0) {
		ExitProgram(); } } }
//////////////////////////////////////////////////////////////////////////////////
//				MAIN PROGRAM					//
//////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv,char** envp) {
	CreateWindow();
	SetupGL();
	InitTimeCounter();
	while(true) {
		UpdateTimeCounter();
		CalculateFPS();
		RotateCube();
		ExposeFunc();
		usleep(1000);
		CheckKeyboard();
	}
	return EXIT_SUCCESS;
}
