CROSS=arm-linux-

all: pwm_test

pwm_test: pwm_test.c
	$(CROSS)gcc -o pwm_test pwm_test.c

clean:
	@rm -vf pwm_test *.o *~
