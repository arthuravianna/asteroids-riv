#include "asteroids.h"
#include <math.h>

void update_asteroid(Asteroid *asteroid) {
    riv_vec2f dir_coords = (riv_vec2f){ sin(asteroid->direction), cos(asteroid->direction) };

    float SPEED = 0.05;
    
    // up makes the spaceship move in the current direction
    riv_vec2f next_asteroid_pos = (riv_vec2f){asteroid->position.x + dir_coords.x*SPEED, asteroid->position.y + dir_coords.y*SPEED};
    
    // make asteroid wrap around the screen
    if (next_asteroid_pos.x < 0) {
        next_asteroid_pos.x = MAP_SIZE;
    } else if (next_asteroid_pos.x > MAP_SIZE) {
        next_asteroid_pos.x = 0;
    }

    if (next_asteroid_pos.y < 0) {
        next_asteroid_pos.y = MAP_SIZE;
    } else if (next_asteroid_pos.y > MAP_SIZE) {
        next_asteroid_pos.y = 0;
    }

    asteroid->position = next_asteroid_pos;
}


void draw_asteroid(Asteroid asteroid) {
    int size = ASTEROIDS_SIZES[asteroid.type];
    riv_draw_rect_fill(asteroid.position.x*TILE_SIZE, asteroid.position.y*TILE_SIZE, size, size, RIV_COLOR_LIGHTRED);
}

void draw_asteroids(List asteroids) {
    Node *node = asteroids.first;
    while (node) {
        Asteroid *asteroid = (Asteroid*)node->data;
        draw_asteroid(*asteroid);

        node = node->next;
    }
}


// Spawn asteroid in a new position
// asteroids come from "outside" of the screen
Asteroid spawn_asteroid() {
    const int asteroid_type = riv_rand_int(BIG_ASTEROID, SMALL_ASTEROID);
    const float asteroid_direction = riv_rand_float();

    // zones
    // 0 = left
    // 1 = top
    // 2 = right
    // 3 = bottom
    const int zone = riv_rand_int(0, 3);
    riv_vec2f asteroid_pos;

    if (zone == 0) {
        asteroid_pos = (riv_vec2f){0.0, riv_rand_float()*MAP_SIZE};

    } else if (zone == 1) {
        asteroid_pos = (riv_vec2f){riv_rand_float()*MAP_SIZE, 0.0};
    } else if (zone == 2) {
        asteroid_pos = (riv_vec2f){MAP_SIZE+1, riv_rand_float()*MAP_SIZE};
    } else {
        asteroid_pos = (riv_vec2f){riv_rand_float()*MAP_SIZE, MAP_SIZE+1};
    }
    
    Asteroid asteroid = { .position = asteroid_pos, .type = asteroid_type, .direction = asteroid_type};
    return asteroid;
}
