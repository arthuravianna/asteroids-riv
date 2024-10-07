#ifndef shots_h
#define shots_h


#include <riv.h>
#include "game.h"
#include "list.h"


#define SHOT_SIZE 2

typedef struct {
    riv_vec2f position;     // position of the shot
    float direction;        // direction of the shot
    int size;
    float speed;
} Shot;

// returns 1 if shot is out of the MAP
// otherwise return 0
bool update_shot(Shot *shot);
void draw_shot(Shot shot);

void update_shots(List *shots);
void draw_shots(List shots);

#endif