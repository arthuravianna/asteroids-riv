#include "asteroids.h"

void update_asteroids(Asteroids *asteroids) {

}


void draw_asteroids(Asteroids asteroids) {
    //riv_draw_rect_fill(asteroid_pos.x*TILE_SIZE, asteroid_pos.y*TILE_SIZE, TILE_SIZE, TILE_SIZE, RIV_COLOR_LIGHTRED);
}

// Spawn asteroid in a new position
bool spawn_asteroid(Asteroids asteroids) {
    riv_vec2i asteroid_pos = (riv_vec2i){riv_rand_uint(MAP_SIZE-1), riv_rand_uint(MAP_SIZE-1)};
    return true;
}

