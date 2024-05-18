
GAME="asteroids"

all: compile-game
	riv-mksqfs ${GAME} ${GAME}.sqfs

compile-game: shots.o spaceship.o asteroids.o
	gcc main.c -o ${GAME} shots.o spaceship.o asteroids.o $(riv-opt-flags -Ospeed) -lriv
	riv-strip ${GAME}

shots.o: shots.c
	gcc -c shots.c

spaceship.o: spaceship.c
	gcc -c spaceship.c

asteroids.o: asteroids.c
	gcc -c asteroids.c