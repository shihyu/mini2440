#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <linux/fs.h>
#include <errno.h>
#include <string.h>

int main(void)
{
	fprintf(stderr, "press Ctrl-C to stop\n");
	int fd = open("/dev/adc", 0);
	if (fd < 0) {
		perror("open ADC device:");
		return 1;
	}
	for(;;) {
		char buffer[30];
		int len = read(fd, buffer, sizeof buffer -1);
		if (len > 0) {
			buffer[len] = '\0';
			int value = -1;
			sscanf(buffer, "%d", &value);
			printf("ADC Value: %d\n", value);
		} else {
			perror("read ADC device:");
			return 1;
		}
		usleep(500* 1000);
	}
	
	close(fd);
}
