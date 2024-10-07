#include "asteroids.h"
#include <math.h>

void update_asteroid(Asteroid *asteroid) {
    riv_vec2f dir_coords = (riv_vec2f){ sin(asteroid->direction), cos(asteroid->direction) };

    float speed = ASTEROIDS_SPEED[asteroid->type];
    
    // up makes the spaceship move in the current direction
    riv_vec2f next_asteroid_pos = (riv_vec2f){asteroid->position.x + dir_coords.x*speed, asteroid->position.y + dir_coords.y*speed};
    
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

    riv_vec2f p0 = asteroid.position;
    riv_vec2f p1;
    riv_vec2f p2;
    riv_vec2f p3;
    riv_vec2f p4;
    riv_vec2f p5;
    riv_vec2f p6;
    riv_vec2f p7;
    if (asteroid.format == FORMAT_0) {
        p1 = (riv_vec2f){p0.x, p0.y + size/2};
        p2 = (riv_vec2f){p0.x + size/4, p0.y + size};
        p3 = (riv_vec2f){p0.x + 3*size/4, p0.y + size};
        p4 = (riv_vec2f){p0.x + 3*size/5, p0.y + size/2};
        p5 = (riv_vec2f){p0.x + size, p0.y + 3*size/4};
        p6 = (riv_vec2f){p0.x + size, p0.y + size/4};
        p7 = (riv_vec2f){p0.x + 3*size/4, p0.y};
    } else if (asteroid.format == FORMAT_1) {
        p1 = (riv_vec2f){p0.x, p0.y + size/3};
        p2 = (riv_vec2f){p0.x + size/4, p0.y + size/2};
        p3 = (riv_vec2f){p0.x, p0.y + size};
        p4 = (riv_vec2f){p0.x + 4*size/5, p0.y + size};
        p5 = (riv_vec2f){p0.x + size, p0.y + 3*size/4};
        p6 = (riv_vec2f){p0.x + size, p0.y + size/3};
        p7 = (riv_vec2f){p0.x + 3*size/4, p0.y};
    } else {

    }

    riv_draw_line(p0.x, p0.y, p1.x, p1.y, RIV_COLOR_WHITE);
    riv_draw_line(p1.x, p1.y, p2.x, p2.y, RIV_COLOR_WHITE);
    riv_draw_line(p2.x, p2.y, p3.x, p3.y, RIV_COLOR_WHITE);
    riv_draw_line(p3.x, p3.y, p4.x, p4.y, RIV_COLOR_WHITE);
    riv_draw_line(p4.x, p4.y, p5.x, p5.y, RIV_COLOR_WHITE);
    riv_draw_line(p5.x, p5.y, p6.x, p6.y, RIV_COLOR_WHITE);
    riv_draw_line(p6.x, p6.y, p7.x, p7.y, RIV_COLOR_WHITE);
    riv_draw_line(p7.x, p7.y, p0.x, p0.y, RIV_COLOR_WHITE);
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
    const int asteroid_format = riv_rand_int(FORMAT_0, FORMAT_1);
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
    
    Asteroid asteroid = { 
        .position = asteroid_pos, 
        .type = asteroid_type, 
        .format = asteroid_format, 
        .direction = asteroid_direction
    };

    return asteroid;
}
