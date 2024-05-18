#include <math.h>
#include <stdio.h>
#include "spaceship.h"

riv_vec2f degrees2coordinate(float degrees) {
    return (riv_vec2f){ sin(degrees), cos(degrees) };
}

void shoot(Spaceship *spaceship) {
    Shot player_shot;
    player_shot.direction = spaceship->direction;
    player_shot.position = (riv_vec2f)spaceship->position;

    push(spaceship->shots, &player_shot, sizeof(Shot));
}

void update_spaceship(Spaceship *spaceship) {
    float angular_speed = M_PI/36; // 5 degrees
    float acceleration = 0.1;

    // left and right change the direction of the spaceship
    if (riv->keys[RIV_GAMEPAD_LEFT].down) {
        spaceship->direction = spaceship->direction + angular_speed;
    } else if (riv->keys[RIV_GAMEPAD_RIGHT].down) {
        spaceship->direction = spaceship->direction - angular_speed;
    }

    riv_vec2f dir_coords = degrees2coordinate(spaceship->direction);
    float magnitude = sqrt(dir_coords.x*dir_coords.x + dir_coords.y*dir_coords.y);
    float unitVectorX = dir_coords.x/magnitude;
    float unitVectorY = dir_coords.y/magnitude;
    
    // up makes the spaceship move in the current direction
    if (riv->keys[RIV_GAMEPAD_UP].down) {
        riv_vec2f next_spaceship_pos = (riv_vec2f){spaceship->position.x + unitVectorX*acceleration, spaceship->position.y + unitVectorY*acceleration};
        
        // check if the next position is valid
        if (next_spaceship_pos.x > 0 && next_spaceship_pos.y > 0 && next_spaceship_pos.x < MAP_SIZE && next_spaceship_pos.y < MAP_SIZE) {
            spaceship->position = next_spaceship_pos;
        }
    }

    // TODO shoot
    if (riv->keys[RIV_GAMEPAD_A1].press) {
        riv_printf("SHOOT\n");
        shoot(spaceship);
    }
    update_shots(spaceship->shots);
}


void draw_spaceship(Spaceship spaceship) {
    riv_vec2f dir_coords = degrees2coordinate(spaceship.direction);

    // draw spaceship info
    char player_info[20];
    sprintf(player_info, "(%f, %f)", spaceship.position.x, spaceship.position.y);
    riv_draw_text(player_info, RIV_SPRITESHEET_FONT_3X5, RIV_TOPLEFT, 0, 5, 1, RIV_COLOR_RED);


    // draw spaceship
    riv_draw_line(spaceship.position.x*(TILE_SIZE) + (TILE_SIZE/2), spaceship.position.y*(TILE_SIZE) + (TILE_SIZE/2), (spaceship.position.x + dir_coords.x) * TILE_SIZE + (TILE_SIZE/2),(spaceship.position.y + dir_coords.y) * TILE_SIZE + (TILE_SIZE/2), RIV_COLOR_BLUE);
    riv_draw_rect_fill(spaceship.position.x*TILE_SIZE, spaceship.position.y*TILE_SIZE, TILE_SIZE, TILE_SIZE, RIV_COLOR_BLUE);

    draw_shots(spaceship.shots);
}