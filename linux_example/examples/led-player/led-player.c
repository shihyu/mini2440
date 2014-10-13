#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>
#include <sys/time.h>
#include <string.h>

static int led_fd;
static int type = 1;

static void push_leds(void)
{
	static unsigned step;
	unsigned led_bitmap;
	int i;

	switch(type) {
	case 0:
		if (step >= 6) {
			step = 0;
		}
		if (step < 3) {
			led_bitmap = 1 << step;
		} else {
			led_bitmap = 1 << (6 - step);
		}
		break;
	case 1:
		if (step > 255) {
			step = 0;
		}
		led_bitmap = step;
		break;
	default:
		led_bitmap = 0;
	}
	step++;
	for (i = 0; i < 4; i++) {
		ioctl(led_fd, led_bitmap & 1, i);
		led_bitmap >>= 1;
	}
}

int main(void)
{
	int led_control_pipe;
	int null_writer_fd; // for read endpoint not blocking when control process exit

	double period = 0.5;

	led_fd = open("/dev/leds0", 0);
	if (led_fd < 0) {
		led_fd = open("/dev/leds", 0);
	}
	if (led_fd < 0) {
		perror("open device leds");
		exit(1);
	}
	unlink("/tmp/led-control");
	mkfifo("/tmp/led-control", 0666);

	led_control_pipe = open("/tmp/led-control", O_RDONLY | O_NONBLOCK);
	if (led_control_pipe < 0) {
		perror("open control pipe for read");
		exit(1);
	}
	null_writer_fd = open("/tmp/led-control", O_WRONLY | O_NONBLOCK);
	if (null_writer_fd < 0) {
		perror("open control pipe for write");
		exit(1);
	}

	for (;;) {
		fd_set rds;
		struct timeval step;
		int ret;

		FD_ZERO(&rds);
		FD_SET(led_control_pipe, &rds);
		step.tv_sec  = period;
		step.tv_usec = (period - step.tv_sec) * 1000000L;

		ret = select(led_control_pipe + 1, &rds, NULL, NULL, &step);
		if (ret < 0) {
			perror("select");
			exit(1);
		}
		if (ret == 0) {
			push_leds();
		} else if (FD_ISSET(led_control_pipe, &rds)) {
			static char buffer[200];
			for (;;) {
				char c;
				int len = strlen(buffer);
				if (len >= sizeof buffer - 1) {
					memset(buffer, 0, sizeof buffer);
					break;
				}
				if (read(led_control_pipe, &c, 1) != 1) {
					break;
				}
				if (c == '\r') {
					continue;
				}
				if (c == '\n') {
					int tmp_type;
					double tmp_period;
					if (sscanf(buffer,"%d%lf", &tmp_type, &tmp_period) == 2) {
						type = tmp_type;
						period = tmp_period;
					}
					fprintf(stderr, "type is %d, period is %lf\n", type, period);
					memset(buffer, 0, sizeof buffer);
					break;
				}
				buffer[len] = c;
			}
		}
	}

	close(led_fd);
	return 0;
}

