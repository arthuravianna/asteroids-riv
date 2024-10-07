#include "shots.h"
#include <math.h>


bool update_shot(Shot *shot) {
    riv_vec2f dir_coords = (riv_vec2f){ sin(shot->direction), cos(shot->direction) };

    float magnitude = sqrt(dir_coords.x*dir_coords.x + dir_coords.y*dir_coords.y);
    float unitVectorX = dir_coords.x/magnitude;
    float unitVectorY = dir_coords.y/magnitude;
    riv_vec2f next_shot_pos = (riv_vec2f){shot->position.x + unitVectorX*shot->speed, shot->position.y + unitVectorY*shot->speed};
    
    // check if the next position is valid
    if (next_shot_pos.x < 0 || next_shot_pos.y < 0 || next_shot_pos.x > MAP_SIZE || next_shot_pos.y > MAP_SIZE) {
        return 1;
    }

    shot->position = next_shot_pos;
    return 0;
}

void draw_shot(Shot shot) {
    riv_draw_rect_line(shot.position.x, shot.position.y, shot.size, shot.size, RIV_COLOR_WHITE);
}



void update_shots(List *shots) {
    Node *node = shots->first;
    Shot *shot;
    int index = 0;
    bool remove = 0;

    while (node) {
        shot = (Shot*)node->data;
        remove = update_shot(shot);
        node = node->next;

        if (remove) {
            removeNode(shots, index);
        } else {
            index++;
        }
    }
}

void draw_shots(List shots) {
    Node *node = shots.first;
    while (node) {
        Shot *shot = (Shot*)node->data;
        draw_shot(*shot);

        node = node->next;
    }
}