#include <ncursesw/ncurses.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <locale.h>

/*
 *      Curses example number 3...
 *
 *      This is an example of how to use wide characters in curses...
 *
 *              Mark Veltzer
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


int main(int argc, char **argv, char **envp) {
	setlocale(LC_ALL, "");

	/*
	 *      printf("size of chtype is %d\n",sizeof(chtype));
	 *      printf("size of wchar_t is %d\n",sizeof(wchar_t));
	 *      printf("size of char is %d\n",sizeof(char));
	 *      printf("size of cchar_t is %d\n",sizeof(cchar_t));
	 */
	initscr();
	cbreak();
	noecho();
	//const wchar_t* string=L"מרק";
	bool stop = false;
	while (!stop) {
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
		 *              res=addwstr(string);
		 *              res=addnwstr(&c,1);
		 *              if(res==ERR) {
		 *                      stop=true;
		 *                      setErr("could not addwstr");
		 *                      continue;
		 *              }
		 *              refresh();
		 */
	}
	endwin();
	printErr();
	return(0);
}
