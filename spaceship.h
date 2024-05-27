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
    int last_shot_frame;
} Spaceship;

void update_spaceship(Spaceship *spaceship);
void draw_spaceship(Spaceship spaceship);
void shoot(Spaceship *spaceship);

#endif