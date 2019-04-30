all: lib

lib: solution.c 
	gcc -shared -fPIC solution.c -o libsolution.so

clean: 
	rm libsolution.so
