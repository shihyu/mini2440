CROSS=arm-linux-

all: armcomtest x86comtest

armcomtest: comtest.c
	$(CROSS)gcc -Wall -O3 -o armcomtest comtest.c
x86comtest: comtest.c
	gcc -o x86comtest comtest.c

clean:
	@rm -vf armcomtest x86comtest *.o *~
