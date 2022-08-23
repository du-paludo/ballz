links = -lm -lallegro_ttf -lallegro_font -lallegro_primitives -lallegro_acodec -lallegro_audio -lallegro_image -lallegro
flags = -Wall -std=c99
files = main.o libqueue.o states.o init.o game.o aux.o
name = ballz

all: $(name)

$(name): main.o $(files)
	gcc -o $(name) $(files) $(flags) $(links)

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
	rm -f $(name)