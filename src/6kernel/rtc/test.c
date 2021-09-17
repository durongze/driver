#include <time.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <linux/rtc.h>
#include <linux/ioctl.h>

#define RTC_DEV "/dev/rtc0"

int print_rtc_time(struct rtc_time *rtc_tm)
{
	printf("Current RTC date/time is %d-%d-%d, %02d:%02d:%02d\n",
		rtc_tm->tm_mday, rtc_tm->tm_mon + 1, rtc_tm->tm_year,
		rtc_tm->tm_hour, rtc_tm->tm_min, rtc_tm->tm_sec);
	return 0;	
}

int init_rtc_time(struct rtc_time *rtc_tm)
{
	rtc_tm->tm_mday = 31;
	rtc_tm->tm_mon = 4;
	rtc_tm->tm_year = 104;
	rtc_tm->tm_hour = 2;
	rtc_tm->tm_min = 30;
	rtc_tm->tm_sec = 0;
	return 0;
}

int main(void)
{
	int rtc_fd;
	int ret;
	struct rtc_time rtc_tm;
	char *rtc_dev = RTC_DEV;
	rtc_fd = open(rtc_dev, O_RDONLY);
	printf("open '%s': %s\n", rtc_dev, strerror(errno));
	if (rtc_fd == -1) {
		exit(1);
	}

	ret = ioctl(rtc_fd, RTC_RD_TIME, &rtc_tm);
	perror("rtc ioctl RTC_RD_TIME");
	print_rtc_time(&rtc_tm);
	
    init_rtc_time(&rtc_tm);
	ret = ioctl(rtc_fd, RTC_SET_TIME, &rtc_tm);
	perror("rtc ioctl RTC_SET_TIME");
	print_rtc_time(&rtc_tm);

	ret = ioctl(rtc_fd, RTC_RD_TIME, &rtc_tm);
	perror("rtc ioctl RTC_RD_TIME");
	print_rtc_time(&rtc_tm);

	close(rtc_fd);
	return 0;
}
