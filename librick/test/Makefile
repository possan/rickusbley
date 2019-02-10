all: roll.o report test8000.wav test44100.wav

clean:
	rm -f roll.o
	rm -f test
	rm -f test8000*
	rm -f test44100*

roll.o: ../src/roll.c ../src/roll.h ../src/loop.inc
	gcc -c -O1 -o roll.o ../src/roll.c

report:
	objdump -d roll.o
	ls -la roll.o

test: test.c roll.o
	gcc -o test test.c roll.o

test.raws: test
	./test

test44100.wav: test.raws
	sox -r 44100 -e signed -b 8 -c 1 test44100.raw test44100.wav

test8000.wav: test.raws
	sox -r 8000 -e signed -b 8 -c 1 test8000.raw test8000.wav
