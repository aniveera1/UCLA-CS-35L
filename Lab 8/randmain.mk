OPTIMIZE = -O2

CC = gcc
CFLAGS = $(OPTIMIZE) -g3 -Wall -Wextra -march=native -mtune=native -mrdrnd

randlibsw.o: randlibsw.c
	$(CC) $(CFLAGS) -fPIC -c randlibsw.c -o randlibsw.o

randlibhw.o: randlibhw.c
	$(CC) $(CFLAGS) -fPIC -c randlibhw.c -o randlibhw.o

randlibsw.so: randlibsw.o
	$(CC) -shared randlibsw.o -o randlibsw.so

randlibhw.so: randlibhw.o
	$(CC) -shared randlibhw.o -o randlibhw.so

randcpuid.o: randcpuid.c
	$(CC) -c randcpuid.c -o randcpuid.o

randmain.o: randmain.c
	$(CC) -c randmain.c

randmain: randmain.o randcpuid.o
	$(CC) -ldl -Wl,-rpath=${PWD} randcpuid.o randmain.o -o randmain
