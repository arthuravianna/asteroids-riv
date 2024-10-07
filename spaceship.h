#ifndef spaceship_h
#define spaceship_h


#include <riv.h>
#include "shots.h"
#include "game.h"
#include "list.h"


typedef struct player_spaceship {
    int lifes;
    riv_vec2f position;     // position of the spaceship
    float direction;        // direction of the spaceship
    int size;
    List *shots;
    float shot_cooldown;
    float shot_speed;
    uint64_t last_shot_frame;
    bool accelerating;
    float acceleration;
    riv_vec2f speed;
    uint64_t collision_frame;
    float collision_cooldown;
} Spaceship;

void update_spaceship(Spaceship *spaceship);
void draw_spaceship(Spaceship spaceship);
void shoot(Spaceship *spaceship);
bool collision_cooldown(Spaceship spaceship);

#endif