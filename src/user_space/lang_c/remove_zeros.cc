#include <stdio.h> // for scanf(3), printf(3), perror(3)

void print_array(int* ar,int size) {
	for(int i=0;i<size;i++)
		printf("ar[%d]=%d\n",i,ar[i]);
}

int remove_zeros(int* ar,int size) {
	int pos=0;
	for(int i=0;i<size;i++) {
		if(ar[i]!=0)
			ar[pos++]=ar[i];
	}
	return pos;
}

int main() {
	// read the size of the array from the user...
	int size;
	printf("give me the size of the array: ");
	int ret=scanf("%d",&size);
	if(ret!=1) {
		perror("scanf");
		return -1;
	}

	// read the array from the user...
	int ar[size];
	for(int i=0;i<size;i++) {
		printf("give me the %d element: ",i);
		//scanf("%d",ar+i);
		int ret=scanf("%d",&ar[i]);
		if(ret!=1) {
			perror("scanf");
			return -1;
		}
	}

	// print the array before...
	print_array(ar,size);
	// remove the zeros...
	int newsize=remove_zeros(ar,size);
	// print the array after...
	printf("*************** results after *********************\n");
	print_array(ar,newsize);
	return 0;
}
