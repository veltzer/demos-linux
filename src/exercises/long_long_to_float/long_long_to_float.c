#include <stdio.h> // for printf(3)

int main(int argc,char** argv,char** envp) {
	long long ll=0;
	// the volatile is needed because otherwise gcc will assume that
	// it knows that back==ll and will not do the actual convesion...
	volatile float f=0;
	long long back=0;
	while(1) {
		f=(float)ll;
		back=(long long)f;
		if(ll!=back) {
			break;
		}
		ll++;
	}
	printf("ll is %lld\n",ll);
	printf("f is %f\n",f);
	printf("back is %lld\n",back);
	return 0;
}
