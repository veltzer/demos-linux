#include <stdio.h> // for printf(3)

int main() {
	long long ll=16777217;
	float f=(float)ll;
	long long back=(long long)f;
	printf("ll is %lld\n",ll);
	printf("f is %f\n",f);
	printf("back is %lld\n",back);
	return 0;
}
