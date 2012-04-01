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

#include<ncursesw/ncurses.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<locale.h>

/*
 * Curses example number 3...
 *
 * This is an example of how to use wide characters in curses...
 *
 * EXTRA_LIBS=-lncursesw
 */

static const char *myMsg = NULL;
void setErr(const char *msg) {
	myMsg = msg;
}

void printErr(void) {
	printf("ERROR: message was [%s]\n", myMsg);
}

int main(int argc,char** argv,char** envp) {
	setlocale(LC_ALL, "");

	/*
	* printf("size of chtype is %d\n",sizeof(chtype));
	* printf("size of wchar_t is %d\n",sizeof(wchar_t));
	* printf("size of char is %d\n",sizeof(char));
	* printf("size of cchar_t is %d\n",sizeof(cchar_t));
	*/
	initscr();
	cbreak();
	noecho();
	//const wchar_t* string=L"מרק";
	bool stop = false;
	while(!stop) {
		wint_t c;
		int res;
		res = get_wch(&c);
		if (res == ERR) {
			stop = true;
			setErr("could not get_wch");
			continue;
		}
		//printw("%d ",c);
		wchar_t str[10];
		swprintf(str, 10, L"%lc", c);
		res = addwstr(str);
		if (res == ERR) {
			stop = true;
			setErr("could not addwstr");
			continue;
		}

		/*
		* res=addwstr(string);
		* res=addnwstr(&c,1);
		* if(res==ERR) {
		*	stop=true;
		*	setErr("could not addwstr");
		*	continue;
		* }
		* refresh();
		*/
	}
	endwin();
	printErr();
	return EXIT_SUCCESS;
}
