#ifndef asteroid_h
#define asteroid_h


#include <riv.h>
#include "game.h"
#include "list.h"


#define BIG_ASTEROID 0
#define MEDIUM_ASTEROID 1
#define SMALL_ASTEROID 2

static int ASTEROIDS_SIZES[] = {32, 16, 8};
static int ASTEROIDS_PNGS[] = {0, 2, 3};
static int ASTEROIDS_SCORE[] = {20, 50, 100};

typedef struct {
    riv_vec2f position;     // position of the asteroid
    float direction;        // direction of the asteroid
    int type;
} Asteroid;


void update_asteroid(Asteroid *asteroid);
void draw_asteroid(Asteroid asteroid);
void draw_asteroids(List asteroids);
Asteroid spawn_asteroid();

#endif