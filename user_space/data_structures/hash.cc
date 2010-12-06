#include <search.h> // for hcreate_r(3), hdestroy_r(3),
#include <stdlib.h> // for exit(3)
#include <stdio.h> // for perror(3)

// the hash table
static hsearch_data HTAB;

int hash_init(void) {
	int res=hcreate_r(100,&HTAB);
	return res;
}

int hash_destroy( void) {
	hdestroy_r(&HTAB);
	return 1;
}

const char* hash_get(const char* key) {
	ENTRY item;
	item.key=(char*)key;
	ENTRY* ritem;
	int ret=hsearch_r(item,FIND,&ritem,&HTAB);
	if(ret==0) {
		return NULL;
	} else {
		return (const char*)(ritem->data);
	}
}
int hash_put(const char* key,const char* data) {
	ENTRY item;
	item.key=(char*)key;
	item.data=(void*)data;
	ENTRY* ritem;
	return hsearch_r(item,ENTER,&ritem,&HTAB);
}
const char* d1_key="mark";
const char* d1_val="veltzer";

int main(int argc,char** argv,char** envp) {
	if(hash_init()==0) {
		perror("hash_init failed");
		exit(1);
	}
	printf("after init\n");
	if(hash_put(d1_key,d1_val)==0) {
		perror("hash_put failed");
		exit(1);
	}
	printf("after put\n");
	const char* getval=hash_get(d1_key);
	printf("got %s\n",getval);
	if(hash_destroy()==0) {
		perror("hash_destroy failed");
		exit(1);
	}
	return 0;
}
