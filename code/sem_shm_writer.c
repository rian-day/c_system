
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<semaphore.h>

#define MSG_FILE "sem_shm_reader.c"
#define SIZE 255
#define PERM S_IRUSR|S_IWUSR
//共享内存通信 信号量
char buf2[SIZE]="This is messageN";
int main(){
	sem_t *sem;
	key_t key;
	int shmid;
	struct shmid_ds buf;
	char *shmaddr2;
	int i;
	sem=sem_open("/mysem",O_CREAT,PERM,0);




	if((key=ftok(MSG_FILE,66))==-1){
		fprintf(stderr,"Creat Key Error:%s\n",strerror(errno));
		exit(EXIT_FAILURE);
	}
	if((shmid=shmget(key,SIZE,PERM|IPC_CREAT))==-1){
		fprintf(stderr,"Creat Sharememory Error:%s\n",strerror(errno));
		exit(EXIT_FAILURE);
	}
	printf("writer:shmaddr=%d\n",shmid);
	if((shmaddr2=(char *)shmat(shmid,NULL,0))==(void *)-1){
		fprintf(stderr,"Attach sharememory error:%s\n",strerror(errno));
		exit(EXIT_FAILURE);
	}
	printf("writer : shmaddr=%s\n",shmaddr2);



	//sem_post();
	for(i=0;i<10;i++){
		buf2[15]=(char)('0'+i);
		strncpy(shmaddr2,buf2,SIZE);
		printf("write %d: %s\n",i,buf2);

	}
	sem_post(sem);
	if(shmdt(shmaddr2)==-1){
		fprintf(stderr,"Detach Sharememory Error:%s\n",strerror(errno));
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);


}
