CROSS=arm-linux-

all: pthread_test

pthread_test:
	$(CROSS)gcc -static -o pthread_test pthread_test.c -lpthread

clean:
	@rm -vf pthread_test *.o *~
