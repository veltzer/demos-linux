/*
 *      This is showing pointer pointer stuff
 *      The idea is to show that the compiler does NOT allow to convert
 *      Foobar** to char**.
 *
 *              Mark Veltzer
 */
void func(char **p) {
}


typedef struct _Foobar {
	int i;
	int j;
} Foobar;

int main(int argc, char **argv, char **envp) {
	// releasing the next two lines will trigger a compilation error!!!
	//Foobar* f=(Foobar*)malloc(sizeof(Foobar));
	//func(&f);
	return(0);
}
