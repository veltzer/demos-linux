extern "C" {
// functions that we need to supply
int cpp_init();
void cpp_exit();

// support functions
void myprintk(const char *);
void *mymalloc(unsigned int);
void myfree(void *);
}

/* to satisfy the kernel dynamic linker */
int __gxx_personality_v0;
int _Unwind_Resume;

/* support code for new and delete */
void *operator new(unsigned int x) {
	return(mymalloc(x));
}


void operator delete(void *pointer) {
	myfree(pointer);
}


class A {
public:
	int x;
	A() {
		myprintk("inside A's constructor!\n");
	}
	~A() {
		myprintk("inside A's destructor!\n");
	}
};

static A *a;

int cpp_init() {
	a = new A();

	return(0);
}

void cpp_exit() {
	delete a;
}
