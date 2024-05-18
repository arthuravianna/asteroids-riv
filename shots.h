#include <riv.h>
#include "game.h"


// #define MAX_SHOTS 16
#define SHOT_SPEED 0.1

typedef struct player_shot {
    riv_vec2f position;     // position of the shot
    float direction;        // direction of the shot
} Shot;

// typedef struct player_shots {
//     Shot shots_arr[MAX_SHOTS];
//     int last_shot;
// } Shots;

// void update_shots(Shots *shots);
// void draw_shots(Shots shots);

void update_shots(List *shots);
void draw_shots(List shots);