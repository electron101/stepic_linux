all: exe lib

exe: solution.c lib
	gcc solution.c -fPIC -ldl -o solution -g

lib: shared.c
	gcc -shared -fPIC shared.c -o libShared.so

clean: 
	-rm hello libShared.so 2>/dev/null
