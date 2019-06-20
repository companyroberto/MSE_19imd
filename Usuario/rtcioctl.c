// SPDX-License-Identifier: GPL-2.0
/*
 * Real Time Clock Periodic Interrupt test program
 *
 * Since commit 6610e0893b8bc ("RTC: Rework RTC code to use timerqueue for
 * events"), PIE are completely handled using hrtimers, without actually using
 * any underlying hardware RTC.
 *
 */

#include <stdio.h>
#include <linux/rtc.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

/*
 * This expects the new RTC class driver framework, working with
 * clocks that will often not be clones of what the PC-AT had.
 * Use the command line to specify another RTC if you need one.
 */
static const char default_rtc[] = "/dev/rtc1";

int main(void)
{
	int fd, retval;
	struct rtc_time rtc_tm;

	const char *rtc = default_rtc;
	fd = open("/dev/rtc1", O_RDONLY);

	if (fd ==  -1) {
		perror(rtc);
		exit(errno);
	}

	/* Read periodic IRQ rate */
	retval = ioctl(fd, RTC_RD_TIME, &rtc_tm);
	if (retval == -1) {
			perror("RTC_RD_TIME ioctl");
			exit(errno);
	}

	fprintf(stderr, "\n\nCurrent RTC date/time is %d-%d-%d, %02d:%02d:%02d.\n",
			rtc_tm.tm_mday, rtc_tm.tm_mon + 1, rtc_tm.tm_year + 1900,
			rtc_tm.tm_hour, rtc_tm.tm_min, rtc_tm.tm_sec);

	close(fd);
	return 0;
}
