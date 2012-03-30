#include<stdio.h> // for printf(3)
#include<sys/types.h> // for opendir(3), closedir(3), rewinddir(3)
#include<dirent.h> // for opendir(3), readdir(3), closedir(3), rewinddir(3)

#include<stdlib.h> // for qsort(3)
#include<string.h> // for strcasecmp(3), strncpy(3)

#include<sys/types.h> // for stat(2), getpwuid(2), for getgrgid(2)
#include<sys/stat.h> // for stat(2)
#include<unistd.h> // for stat(2)
#include<pwd.h> // for getpwuid(2)
#include<grp.h> // for getgrgid(2)

#include<math.h> // for log10(3), lround(3)

#include"us_helper.hh" // our own helper

/*
 * TODO:
 * - print date 
 * - print symlink.
 */

static int strcmp_wrap(const void* pa,const void* pb) {
	const char** ppa=(const char**)pa;
	const char** ppb=(const char**)pb;
	return strcasecmp(*ppa,*ppb);
}

static void filetype(mode_t m,char* p) {
	if(S_ISREG(m)) {
		p[0]='-';
	}
	if(S_ISDIR(m)) {
		p[0]='d';
	}
	if(S_ISCHR(m)) {
		p[0]='c';
	}
	if(S_ISBLK(m)) {
		p[0]='b';
	}
	if(S_ISFIFO(m)) {
		p[0]='p';
	}
	if(S_ISLNK(m)) {
		p[0]='l';
	}
	if(S_ISSOCK(m)) {
		p[0]='s';
	}
	p[1]='-';
	p[2]='-';
	p[3]='-';
	p[4]='-';
	p[5]='-';
	p[6]='-';
	p[7]='-';
	p[8]='-';
	p[9]='-';
	if(S_IRUSR & m) {
		p[1]='r';
	}
	if(S_IWUSR & m) {
		p[2]='w';
	}
	if(S_IXUSR & m) {
		p[3]='x';
	}
	if(S_IRGRP & m) {
		p[4]='r';
	}
	if(S_IWGRP & m) {
		p[5]='w';
	}
	if(S_IXGRP & m) {
		p[6]='x';
	}
	if(S_IROTH & m) {
		p[7]='r';
	}
	if(S_IWOTH & m) {
		p[8]='w';
	}
	if(S_IXOTH & m) {
		p[9]='x';
	}
	p[10]='\0';
}

int main(int argc,char** argv,char** envp) {
	DIR* d;
	struct dirent* dircontent;
	const bool hidedots=true;
	CHECK_NOT_NULL(d=opendir("."));
	int num_files=0;
	int num_blocks=0;
	while ((dircontent = readdir(d))) {
		if(hidedots && dircontent->d_name[0]=='.') {
			continue;
		}
		num_files++;
	}
	char** arr=(char**)malloc(sizeof(char*)*num_files);
	int i=0;
	rewinddir(d);
	int max_size=0;
	nlink_t max_link=0;
	while((dircontent = readdir(d))) {
		if(hidedots && dircontent->d_name[0]=='.') {
			continue;
		}
		arr[i]=(char*)malloc(256);
		strncpy(arr[i],dircontent->d_name,256);
		struct stat buf;
		CHECK_NOT_M1(stat(arr[i],&buf));
		num_blocks+=buf.st_blocks;
		if(buf.st_nlink>max_link) {
			max_link=buf.st_nlink;
		}
		if(buf.st_size>max_size) {
			max_size=buf.st_size;
		}
		i++;
	}
	//printf("log10 is %lf, %ld, %d\n",log10(max_link),lrint(ceil(log10(max_link))),max_link);
	int size_width=lrint(ceil(log10(max_size)));
	int size_link=lrint(ceil(log10(max_link+1)));
	CHECK_NOT_M1(closedir(d));
	qsort(arr,num_files,sizeof(char*),strcmp_wrap);
	printf("total %d\n",num_blocks/2);
	for(int i=0;i<num_files;i++) {
		// dont show . files...
		if(hidedots && arr[i][0]=='.') {
			continue;
		}
		struct stat buf;
		CHECK_NOT_M1(stat(arr[i],&buf));
		char p[11];
		mode_t m=buf.st_mode;
		filetype(m,p);
		struct passwd *pwd=getpwuid(buf.st_uid);
		struct group* grp=getgrgid(buf.st_gid);
		printf("%s %*d %s %s %*ld %s\n",
			p,
			size_link,
			buf.st_nlink,
			pwd->pw_name,
			grp->gr_name,
			size_width,
			buf.st_size,
			arr[i]
		);
	}
	return 0;
}
