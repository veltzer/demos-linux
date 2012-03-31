#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<curses.h>
#include<unistd.h>

/*
 * Curses example number 1...
 *
 * This is an example of how to read a character from the keyboard
 * in non-blocking mode...
 *
 *		Mark Veltzer
 *
 * EXTRA_LIBS=-lncursesw
 */
int main(int argc, char **argv, char **envp) {
	initscr();
	cbreak();
	noecho();
	while(true) {
		int c = getch();
		if (c == ERR) {
			// there is no input on the keyboard. Wait a while to avoid busy wait...
			//usleep(100000);
		} else {
			// We have a character!!! Print it on the screen...
			echochar(c);
			//printf("char is %d\n",c);
		}
	}
	endwin();
	return EXIT_SUCCESS;
}
