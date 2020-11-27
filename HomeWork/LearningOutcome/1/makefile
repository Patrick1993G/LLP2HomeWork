all: clean build
buildNormal:
	gcc -Wall -g -O0 -o counter -DNORMAL counter.c
buildDef:
	gcc -Wall -g -O0 -o counter -Dc counter.c 
buildExtended:
	gcc -Wall -g -O0 -o counter2  -Dc -DEXTENDED counter.c
clean:
	-rm counter
	-rm counter2

