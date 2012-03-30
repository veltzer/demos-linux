#include"Phil.hh"

int semid;

void think(int id)
{
	int stime;
	printf("Philosopher %d is busy thinking\n", id);
	srand(time(0));
	stime=1+(int) (10.0*rand()/(RAND_MAX+1.0));
	sleep(stime);
	printf("Philosopher %d finished thinking\n", id);
}

void eat(int id)
{
	int stime;
	printf("Philosopher %d is busy eating\n", id);
	srand(time(0));
	stime=1+(int) (10.0*rand()/(RAND_MAX+1.0));
	sleep(stime);
	printf("Philosopher %d finished eating\n", id);
}

void putForks(int id)
{
	int next;
	if(id == NPHIL-1)
		next=0;
	else
		next=id+1;
	printf("Philosopher %d is replacing the forks\n", id);
	struct sembuf sops[2];
	sops[0].sem_num = id;
	sops[0].sem_op = 1;
	sops[0].sem_flg = 0;
	sops[1].sem_num = next;
	sops[1].sem_op = 1;
	sops[1].sem_flg = 0;
	if ( semop(semid, sops, 2) == -1 )
	{
		perror("semop");
		exit(errno);
	}
	printf("Philosopher %d replaced the forks\n", id);
}

void pickForks(int id)
{
	int next;
	if(id == NPHIL-1)
		next=0;
	else
		next = id+1;
	printf("Philosopher %d is trying to pick up the forks\n", id);
	struct sembuf sops[2];
	sops[0].sem_num = id;
	sops[0].sem_op = -1;
	sops[0].sem_flg = 0;
	sops[1].sem_num = next;
	sops[1].sem_op = -1;
	sops[1].sem_flg = 0;
	if ( semop(semid, sops, 2) == -1 )
	{
		perror("semop");
		exit(errno);
	}
	printf("Philosopher %d finally picked the forks\n", id);
}

int main(int argc,char** argv,char** envp)
{
	key_t key;
	int id;
	if(argc != 2)
	{
		fprintf(stderr, "Usage: %s [0-%d]\n", argv[0], NPHIL-1);
		exit(1);
	}
	if((argv[1][0] >= '0') && (argv[1][0] <='5') && (strlen(argv[1]) == 1))
		id=atoi(argv[1]);
	else
	{
		fprintf(stderr, "bad argument %c, Argument must be numeric value between 0 and 5\n", argv[1][0]);
		exit(1);
	}
	
	if ((key = ftok(KEYFILE, 'x')) == -1 )
	{
		perror("ftok failed");
		exit(errno);
	}

	if ((semid = semget(key, 0, 0)) == -1 )
	{
		perror("semget");
		exit(errno);
	}

	while(1)
	{
		pickForks(id);
		eat(id);
		putForks(id);
		think(id);
	}
	exit(0);
}
