#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#define PERROR(x) perror("[" __FILE__ "]" x)
#define PDEBUG(fmt, args...) fprintf(stderr, "[%s:%d]" fmt, __FILE__, __LINE__, ## args)

int main(int argc, char** argv)
{
	int fd;
	char num[2];

	fd = open("/dev/fgj", O_RDWR, S_IRUSR | S_IWUSR);
	PERROR("open");
	if (fd == - 1)
	{
		return -1;
	}
	while (1)
	{
		read(fd, &num,2); //程序将阻塞在此语句
		PDEBUG("The data is %s,quit(num[1] == 0x35)\n", num);
		if (num[1] == 0x35)
		{
			close(fd);
			PDEBUG("%s quit\n", argv[0]);
			break;
		}
	}
	return 0;
}
