allegro_flags = `pkg-config allegro-5 allegro_font-5 allegro_ttf-5 allegro_primitives-5 allegro_audio-5 allegro_acodec-5 allegro_image-5 --libs --cflags`
flags = -Wall -std=c99 -lm
files = main.o libqueue.o states.o init.o game.o aux.o
name = ballz

all: ballz

ballz: main.o $(files)
	gcc -o $(name) $(files) $(flags) $(allegro_flags)

main.o: main.c
	gcc -c main.c $(flags)

libqueue.o: libqueue.c
	gcc -c libqueue.c $(flags)

states.o: states.c
	gcc -c states.c $(flags)

init.o: init.c
	gcc -c init.c $(flags)

game.o: game.c
	gcc -c game.c $(flags)

aux.o: aux.c
	gcc -c aux.c $(flags)

clean:
	rm -f *~ *.o

purge: clean
	rm -f $(nome)