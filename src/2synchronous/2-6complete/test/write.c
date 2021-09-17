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
	int i=0;

	fd = open("/dev/fgj", O_RDWR, S_IRUSR | S_IWUSR);
	PERROR("open");
	if (fd == - 1)
	{
		return -1;
	}
    num[0]='F';
	num[1]='G' - 0x12;
	write(fd, num, sizeof(num));
	PDEBUG("%s -> %s 0x%02X\n", argv[0], num, num[1]);
	close(fd);
	return 0;
}
