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
        printf("%s <dev>, but argc:%d\n", argv[0], argc);
        return 1;
    }
	int fd = open(argv[1], O_RDONLY);
    if (-1 == fd) {
		perror("open");
        return 2;
    }
    ioctl(fd, EVIOCGVERSION, &version);
	printf("version:0x%X\n", version);

    struct input_devinfo device_info;
    ioctl(fd, EVIOCGID, &device_info);
	printf("dev_info: bustype:0x%X, vendor:0x%X, product:0x%X, version:0x%X\n",
	    device_info.bustype, device_info.vendor, device_info.product, device_info.version);

    char name[256]= "Unknown";
    ioctl(fd, EVIOCGNAME(sizeof(name)), name);
	printf("name: %s\n", name);

    uint8_t rel_bitmask[REL_MAX/8 + 1];
    ioctl(fd, EVIOCGBIT(EV_REL, sizeof(rel_bitmask)));
	printf("rel_bitmask:");
	for (int i = 0; i < sizeof(rel_bitmask)/sizeof(rel_bitmask[0]); ++i) {
		printf("%02X ", rel_bitmask[i]);
	}
	printf("\n");
    return 0;
}
