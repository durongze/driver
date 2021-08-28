#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/input.h>
#include <stdio.h>
struct input_devinfo {
    __u16 bustype;
    __u16 vendor;
    __u16 product;
    __u16 version;
};
int main (int argc, char** argv) 
{
    int version;
    if (argc != 2) {
        printf("%s <dev>", argv[0]);
        return 1;
    }
	int fd = open(argv[1], O_RDONLY);
    if (-1 == fd) {
        return 2;
    }
    ioctl(fd, EVIOCGVERSION, &version);
    struct input_devinfo device_info;
    ioctl(fd, EVIOCGID, &device_info);
    char name[256]= "Unknown";
    ioctl(fd, EVIOCGNAME(sizeof(name)), name);
    uint8_t rel_bitmask[REL_MAX/8 + 1];
    ioctl(fd, EVIOCGBIT(EV_REL, sizeof(rel_bitmask)));
    return 0;
}
