all: exe lib

exe: hello.h main.c lib
	gcc main.c -fPIC -ldl -o hello -g

lib: hello.h hello.c
	gcc -shared -fPIC hello.c -o libHello.so

clean: 
	-rm hello libHello.so 2>/dev/null
