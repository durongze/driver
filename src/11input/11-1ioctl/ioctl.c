#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/input.h>
#include <stdio.h>

#define PERROR(x) perror("[" __FILE__ "]" x)
#define PDEBUG(fmt, args...) printf( "[%s:%d]:" fmt, __FUNCTION__, __LINE__, ## args)

struct input_devinfo {
    __u16 bustype;
    __u16 vendor;
    __u16 product;
    __u16 version;
};

int print_input_devinfo(struct input_devinfo *device_info)
{
	PDEBUG("dev_info: bustype:0x%X, vendor:0x%X, product:0x%X, version:0x%X\n",
	    device_info->bustype, device_info->vendor, device_info->product, device_info->version);
	return 0;
}

int print_rel_bitmask(uint8_t *rel_bitmask, size_t size)
{
	PDEBUG("rel_bitmask:");
	for (int i = 0; i < size; ++i) {
		printf("%02X ", rel_bitmask[i]);
	}
	printf("\n");
	return 0;
}

int main (int argc, char** argv) 
{
    int version;
    if (argc != 2) {
        PDEBUG("%s <dev>, but argc:%d\n", argv[0], argc);
        return 1;
    }
	int fd = open(argv[1], O_RDONLY);
    if (-1 == fd) {
		PERROR("open");
        return 2;
    }
    ioctl(fd, EVIOCGVERSION, &version);
	PDEBUG("version:0x%X\n", version);

    struct input_devinfo device_info;
    ioctl(fd, EVIOCGID, &device_info);
	print_input_devinfo(&device_info);

    char name[256]= "Unknown";
    ioctl(fd, EVIOCGNAME(sizeof(name)), name);
	PDEBUG("name: %s\n", name);

    uint8_t rel_bitmask[REL_MAX/8 + 1];
    ioctl(fd, EVIOCGBIT(EV_REL, sizeof(rel_bitmask)));
	print_rel_bitmask(rel_bitmask, sizeof(rel_bitmask) / sizeof(rel_bitmask[0]));
    return 0;
}
