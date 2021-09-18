#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>
#include<linux/rtc.h>
#include<linux/ioctl.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>

#define CMD_LEN 4
#define CMD_QUIT "quit"
#define CMD_DATA "data"

#define PERROR(fmt) perror( __FILE__ fmt)
#define PDEBUG(fmt, args...) printf( "[%s:%d]" fmt, __FUNCTION__, __LINE__, ## args)
int fd;

void *readthread(void *arg)
{
	char data[256];
    fd_set rfds; //¶ÁÃèÊö·û¼¯ºÏ
    fd_set wfds; //Ð´ÃèÊö·û¼¯ºÏ
	int retval=0;
	while(1)
	{
		FD_ZERO(&rfds);
        FD_SET(fd, &rfds);
		select(fd+1, &rfds, &wfds, NULL, NULL);		
		if(FD_ISSET(fd, &rfds))
		{
			retval=read(fd,data, CMD_LEN);
			sleep(1);
			PERROR("read");
			if(retval==-1) {
				exit(-1);
			}
			data[retval]=0;
			PDEBUG("read data:%s\n",data);
			if (strncmp(data, CMD_QUIT, strlen(CMD_QUIT)) == 0) {
				break;
			}
		}
	}
    return (void *)0;
} 

int main(int argc, char** argv)
{
	
	int i;
	int retval;
	fd=open("/dev/fgj",O_RDWR);
	PERROR("open\n");
	if(fd==-1) {
		exit(-1);
	}
	
	pthread_t tid;
	pthread_create(&tid, NULL, readthread, NULL);
	
	for (i = 0; i < 21; ++i) {
		retval=write(fd, CMD_DATA, strlen(CMD_DATA));
		PDEBUG("write\n");
		sleep(1);
		if(retval == -1) {
			exit(-1);
		}
	}
	
	retval=write(fd, CMD_QUIT, strlen(CMD_QUIT));
	pthread_join(tid, NULL);
	close(fd);
	return 0;
}
