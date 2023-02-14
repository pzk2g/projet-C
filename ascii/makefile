CC=gcc

all : pixel
pixel : jpg2ascii.o reverse.o pixel_art.o artist.o
	$(CC) -o pixel jpg2ascii.o reverse.o pixel_art.o artist.o -lturbojpeg -lm
jpg2ascii.o : jpg2ascii.c conversion.h
	$(CC) 	-c jpg2ascii.c  -lm
reverse.o: reverse.c conversion.h
	$(CC)   -c reverse.c -lm
pixel_art.o: pixel_art.c conversion.h
	$(CC)   -c pixel_art.c -lm
artist.o : jpg2ascii.c reverse.c pixel_art.c artist.c conversion.h
	$(CC) 	-c artist.c -lm
clean :
	rm *.o pixel core
