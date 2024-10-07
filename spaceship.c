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
    player_shot.size = SHOT_SIZE;
    player_shot.speed = spaceship->shot_speed;
    
    push(spaceship->shots, &player_shot, sizeof(Shot));
}

void update_spaceship(Spaceship *spaceship) {
    float angular_speed = M_PI/36; // 5 degrees
    riv_vec2f dir_coords = degrees2coordinate(spaceship->direction);

    // left and right change the direction of the spaceship
    if (riv->keys[RIV_GAMEPAD_LEFT].down) {
        spaceship->direction = spaceship->direction + angular_speed;
    } else if (riv->keys[RIV_GAMEPAD_RIGHT].down) {
        spaceship->direction = spaceship->direction - angular_speed;
    }

    riv_vec2f next_spaceship_pos = (riv_vec2f){spaceship->position.x + spaceship->speed.x, spaceship->position.y + spaceship->speed.y};

    // make spaceship wrap around the screen
    if (next_spaceship_pos.x < 0) {
        next_spaceship_pos.x = MAP_SIZE;
    } else if (next_spaceship_pos.x > MAP_SIZE) {
        next_spaceship_pos.x = 0;
    }

    if (next_spaceship_pos.y < 0) {
        next_spaceship_pos.y = MAP_SIZE;
    } else if (next_spaceship_pos.y > MAP_SIZE) {
        next_spaceship_pos.y = 0;
    }
    spaceship->position = next_spaceship_pos;
    
    // up accelerates the spaceship in the current direction
    if (!riv->keys[RIV_GAMEPAD_UP].down) {
        spaceship->accelerating = false;
    } else {
        spaceship->accelerating = true;
        riv_vec2f ac = (riv_vec2f){spaceship->acceleration*dir_coords.x, spaceship->acceleration*dir_coords.y};        
        
        // update speed
        spaceship->speed.x = spaceship->speed.x + ac.x;
        spaceship->speed.y = spaceship->speed.y + ac.y;
    }

    bool shot_cooldown = riv->frame - spaceship->last_shot_frame < spaceship->shot_cooldown*riv->target_fps;
    if (riv->keys[RIV_GAMEPAD_A1].press && !shot_cooldown) {
        shoot(spaceship);
        spaceship->last_shot_frame = riv->frame;
    }
}

// rotate a point "p" around another point "c" by "θ" degrees
riv_vec2f rotate(riv_vec2f p, riv_vec2f c, float theta) {
    // p′x = cx + (px − cx)⋅cosθ − (py − cy)⋅sinθ
    // p′y = cy + (px − cx)⋅sinθ + (py − cy)⋅cosθ

    double new_px = c.x + (p.x - c.x)*cos(theta) - (p.y - c.y)*sin(theta);
    double new_py = c.y + (p.x - c.x)*sin(theta) + (p.y - c.y)*cos(theta);

    return (riv_vec2f){new_px, new_py};
}

void draw_spaceship(Spaceship spaceship) {
    // rotate in the opposite direction
    float theta = spaceship.direction * -1;

    double spaceship_cx = spaceship.position.x;
    double spaceship_cy = spaceship.position.y;

    riv_vec2f spaceship_p0 = (riv_vec2f){spaceship_cx, spaceship_cy + spaceship.size};
    riv_vec2f spaceship_p01 = rotate(spaceship_p0, spaceship.position, theta);


    riv_vec2f spaceship_p1 = (riv_vec2f){spaceship_cx - (spaceship.size/2), spaceship_cy - spaceship.size};
    riv_vec2f spaceship_p11 = rotate(spaceship_p1, spaceship.position, theta);

    riv_vec2f spaceship_p2 = (riv_vec2f){spaceship_cx + (spaceship.size/2), spaceship_cy - spaceship.size};
    riv_vec2f spaceship_p21 = rotate(spaceship_p2, spaceship.position, theta);

    // make spaceship blink when on collision cooldown
    uint32_t spaceship_color = (collision_cooldown(spaceship) && riv->frame % 5 == 0) ? RIV_COLOR_BLACK : RIV_COLOR_WHITE;
    riv_draw_triangle_line(
        spaceship_p01.x, spaceship_p01.y,
        spaceship_p11.x, spaceship_p11.y,
        spaceship_p21.x, spaceship_p21.y,
        spaceship_color
    );

    // if accelerating draw engine fire
    if (spaceship.accelerating) {
        riv_vec2f engine_c = (riv_vec2f){(spaceship_p11.x + spaceship_p21.x)/2, (spaceship_p11.y + spaceship_p21.y)/2};

        riv_vec2f engine_p0 = (riv_vec2f){engine_c.x, engine_c.y - 5};
        riv_vec2f engine_p01 = rotate(engine_p0, engine_c, theta);

        riv_vec2f engine_p1 = (riv_vec2f){((spaceship_p11.x + spaceship_p21.x)/2) + spaceship.size/4, (spaceship_p11.y + spaceship_p21.y)/2};
        riv_vec2f engine_p11 = rotate(engine_p1, engine_c, theta);
        
        riv_vec2f engine_p2 = (riv_vec2f){((spaceship_p11.x + spaceship_p21.x)/2) - spaceship.size/4, (spaceship_p11.y + spaceship_p21.y)/2};
        riv_vec2f engine_p21 = rotate(engine_p2, engine_c, theta);

        riv_draw_triangle_line(
            engine_p01.x, engine_p01.y,
            engine_p11.x, engine_p11.y,
            engine_p21.x, engine_p21.y,
            spaceship_color
        );
    }
}

bool collision_cooldown(Spaceship spaceship) {
    bool cooldown = riv->frame - spaceship.collision_frame < spaceship.collision_cooldown*riv->target_fps;
    return cooldown;
}