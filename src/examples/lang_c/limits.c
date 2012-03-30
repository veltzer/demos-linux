#include<stdio.h> // for printf(3)

int main() {
	long long ll=0;
	long long back_value=0;
	while(back_value==ll) {
		ll++;
		float f=(float)ll;
		back_value=(long long)f;
	}
	printf("ll is %lld\n",ll);
	return 0;
}
