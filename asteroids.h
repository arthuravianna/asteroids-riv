#ifndef asteroid_h
#define asteroid_h


#include <riv.h>
#include "game.h"
#include "list.h"

// asteroids types
#define BIG_ASTEROID 0
#define MEDIUM_ASTEROID 1
#define SMALL_ASTEROID 2

// asteroid formats
enum asteroid_format {
  FORMAT_0,
  FORMAT_1,
  FORMAT_2
};

static int ASTEROIDS_SIZES[] = {32, 16, 8};
static float ASTEROIDS_SPEED[] = {0.1, 0.4, 1};
static int ASTEROIDS_SCORE[] = {20, 50, 100};

typedef struct {
    riv_vec2f position;     // position of the asteroid
    float direction;        // direction of the asteroid
    int type;
    enum asteroid_format format;
} Asteroid;


void update_asteroid(Asteroid *asteroid);
void draw_asteroid(Asteroid asteroid);
void draw_asteroids(List asteroids);
Asteroid spawn_asteroid();

#endif