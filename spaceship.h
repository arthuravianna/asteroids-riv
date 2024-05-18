#include <riv.h>
#include "shots.h"
#include "game.h"
#include "list.h"


typedef struct player_spaceship {
    int lifes;
    riv_vec2f position;     // position of the spaceship
    float direction;        // direction of the spaceship
    //Shot shots;
    List *shots;
} Spaceship;

void update_spaceship(Spaceship *spaceship);
void draw_spaceship(Spaceship spaceship);
