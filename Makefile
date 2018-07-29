CFLAGS=-Wall
fjpeg_test: test.o fjpeg.o move_detect.o
	gcc -o fjpeg_test test.o fjpeg.o move_detect.o

clean:
	rm *.o