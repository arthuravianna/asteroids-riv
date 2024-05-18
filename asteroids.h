#include <riv.h>
#include "game.h"


#define MAX_ASTEROIDS 30

struct Asteroid {
    riv_vec2i position;     // position of the asteroid
    riv_vec2i direction;    // direction of the asteroid
};


typedef struct asteroids_obstacles {
    struct Asteroid asteroids[MAX_ASTEROIDS];
    int last_shot;
} Asteroids;

void update_asteroids(Asteroids *asteroids);
void draw_asteroids(Asteroids asteroids);

