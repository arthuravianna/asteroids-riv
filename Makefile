
GAME="asteroids"
GAME_DIR="src"
GAME_BIN="out"
GAME_FILES=$(shell ls -X --reverse ${GAME_DIR}/*.[c,h])
OPTIMIZE_BY="speed"

all: create-cartridge

run: create-cartridge
	rivemu ${GAME}.sqfs

# run-debug:
# 	rivemu -quiet -no-window -sdk -workspace -exec 'gcc main.c -o ${GAME}-debug $(riv-opt-flags -Odebug) -lriv'
# 	rivemu -quiet -sdk -workspace -it -exec gdb -silent ./${GAME}-debug

run-benchmark: create-cartridge
	rivemu -quiet -bench ${GAME}.sqfs

inspect-cartridge:
	rivemu -quiet -no-window -sdk -workspace -exec 'ls -l ${GAME}.sqfs && file ${GAME}.sqfs'

# compiled (default)
create-cartridge: compile-game
	rivemu -quiet -no-window -sdk -workspace -exec riv-mksqfs ${GAME} ${GAME}.sqfs

compile-game: list.o shots.o spaceship.o asteroids.o
	@echo "Optimizing by: ${OPTIMIZE_BY}"
#rivemu -quiet -no-window -sdk -workspace -exec 'gcc main.c -o ${GAME} $(riv-opt-flags -O${OPTIMIZE_BY}) -lriv'
	rivemu -quiet -no-window -sdk -workspace -exec 'gcc main.c -o ${GAME} list.o shots.o spaceship.o asteroids.o $(riv-opt-flags -Ospeed) -lriv'
	rivemu -quiet -no-window -sdk -workspace -exec riv-strip ${GAME}

list.o: list.c
	rivemu -quiet -no-window -sdk -workspace -exec 'gcc -c list.c'

shots.o: shots.c
	rivemu -quiet -no-window -sdk -workspace -exec 'gcc -c shots.c'

spaceship.o: spaceship.c
	rivemu -quiet -no-window -sdk -workspace -exec 'gcc -c spaceship.c'

asteroids.o: asteroids.c
	rivemu -quiet -no-window -sdk -workspace -exec 'gcc -c asteroids.c'


clean:
	rm -f ${GAME}
	rm -f ${GAME}.sqfs
	rm *.o