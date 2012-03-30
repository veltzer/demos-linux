#include<stdio.h>
#include<assert.h> // for assert(3)
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<X11/Xlib.h>
#include<X11/extensions/Xrandr.h>

/*
 *	OpenGL example number 3...
 *	Taken from http://www.opengl.org/wiki/Programming_OpenGL_in_Linux:_Changing_the_Screen_Resolution
 *
 *		Mark Veltzer
 *
 * EXTRA_LIBS=-lX11 -lXrandr
 */

int main(int argc,char** argv,char** envp) {
	//
	// CONNECT TO X-SERVER, GET ROOT WINDOW ID
	//
	Display* dpy=XOpenDisplay(NULL);
	assert(dpy!=NULL);
	// TODO: How do I know if the next function call was successful?
	Window root=DefaultRootWindow(dpy);
	int event_base_return,error_base_return;
	Bool res=XRRQueryExtension(dpy,&event_base_return,&error_base_return);
	assert(res);
	int major_version_return,minor_version_return;
	Status s=XRRQueryVersion(dpy,&major_version_return,&minor_version_return);
	assert(s);
	printf("major_version_return, minor_version_return is %d,%d\n",major_version_return,minor_version_return);
	//
	//	GET POSSIBLE SCREEN RESOLUTIONS
	//
	int num_sizes;
	XRRScreenSize* xrrs=XRRSizes(dpy, 0, &num_sizes);
	//
	//	LOOP THROUGH ALL POSSIBLE RESOLUTIONS,
	//	GETTING THE SELECTABLE DISPLAY FREQUENCIES
	//
	// to store all possible frequencies...
	short possible_frequencies[64][64];
	for(int i = 0; i < num_sizes; i++) {
		printf("%i : %i x %i (%immx%imm)", i, xrrs[i].width, xrrs[i].height, xrrs[i].mwidth, xrrs[i].mheight);
		// print the rates of each size
		int num_rates;
		short* rates=XRRRates(dpy,0,i,&num_rates);
		for(int j=0;j<num_rates;j++) {
			possible_frequencies[i][j] = rates[j];
			printf(" %i", rates[j]);
		}
	}
	printf("\n");
	//
	//	GET CURRENT RESOLUTION AND FREQUENCY
	//
	XRRScreenConfiguration* conf=XRRGetScreenInfo(dpy,root);
	short original_rate=XRRConfigCurrentRate(conf);
	Rotation original_rotation;
	SizeID original_size_id=XRRConfigCurrentConfiguration(conf,&original_rotation);

	printf("CURRENT SIZE ID : %i\n", original_size_id);
	printf("CURRENT ROTATION : %i \n", original_rotation);
	printf("CURRENT RATE : %i Hz\n\n", original_rate);

	assert(num_sizes>1);
	//
	//	CHANGE RESOLUTION
	//
	printf("CHANGED TO %i x %i PIXELS, %i Hz\n\n", xrrs[1].width, xrrs[1].height, possible_frequencies[1][0]);
	XRRSetScreenConfigAndRate(dpy, conf, root, 1, RR_Rotate_0, possible_frequencies[1][0], CurrentTime);
	//
	//	SLEEP A WHILE
	//
	usleep(6000000);
	//
	//	RESTORE ORIGINAL CONFIGURATION
	//
	printf("RESTORING %i x %i PIXELS, %i Hz\n\n", xrrs[original_size_id].width, xrrs[original_size_id].height, original_rate);
	XRRSetScreenConfigAndRate(dpy, conf, root, original_size_id, original_rotation, original_rate, CurrentTime);
	//
	//	EXIT
	//
	assert(XCloseDisplay(dpy));
	return EXIT_SUCCESS;
}
