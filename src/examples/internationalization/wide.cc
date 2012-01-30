#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <locale.h>

/*
 *      This example explores wide strings in GCC.
 *
 *              Mark Veltzer
 */
#define WS(s) L ## s

int main(int argc, char **argv, char **envp) {
	setlocale(LC_ALL, "");
	wprintf(L"size of char is %d\n", sizeof(char));
	wprintf(L"size of wchar_t is %d\n", sizeof(wchar_t));
	//const char* s1="hello";
	const wchar_t *s2 = L"קרמ\n";
	//const wchar_t* s3=WS("מרק");
	int ret = fputws(s2, stdout);
	wprintf(L"return is %d\n", ret);
	//wprintf(L"here is a string\n");
	//printf("length of %s is %d\n",s1,strlen(s1));
	wprintf(L"here is a wide char for ya [%ls]\n", s2);
	return(0);
}
