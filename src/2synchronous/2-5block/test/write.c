#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define PERROR(x) perror("[" __FILE__ "]" x)
#define PDEBUG(fmt, args...) fprintf(stderr, "[%s:%d]" fmt, __FILE__, __LINE__, ## args)

int main(int argc, char** argv)
{
	int fd;
	char num[2];
	int i=0;

	fd = open("/dev/fgj", O_RDWR, S_IRUSR | S_IWUSR);
	PERROR("open");
	if (fd != - 1)
	{
        num[0]='F';
		num[1]='G' - 0x12;
		PDEBUG("%s -> 0x%02X\n", argv[0], num[1]);
		write(fd, num, 2);
		close(fd);
	}
	exit(0);
}
