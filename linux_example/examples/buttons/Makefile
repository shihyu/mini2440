CROSS=arm-linux-

all: buttons

buttons: buttons_test.c
	$(CROSS)gcc -o buttons buttons_test.c

clean:
	@rm -vf buttons *.o *~
