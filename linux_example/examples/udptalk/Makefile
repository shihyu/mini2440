CROSS=arm-linux-

all: arm-udptalk x86-udptalk

arm-udptalk:
	$(CROSS)gcc -o arm-udptalk udptalk.c
x86-udptalk:
	gcc -o x86-udptalk udptalk.c
clean:
	@rm -vf *-udptalk *.o *~
