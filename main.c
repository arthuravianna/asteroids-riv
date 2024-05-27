#include <stdio.h>
#include <math.h>
#include <riv.h>
#include "game.h"
#include "spaceship.h"
#include "asteroids.h"

enum GAME_STATE {
    MENU,
    PLAYING,
    ENDED
};

enum GAME_STATE game_state;

Spaceship player;
List *asteroids;
int score = 0;
float spawn_probability = 0.05;
int spawn_cooldown = 2; // in seconds
int last_spawn_frame = 0;
int probability_increase_cooldown = 5;
int last_frame = 0; // used to count time in seconds

////////////
// draw
////////////
void draw_menu_screen() {
    // Draw snake title
    riv_draw_text(
        "Asteroids",              // text to draw
        RIV_SPRITESHEET_FONT_5X7, // sprite sheet id of the font
        RIV_CENTER,               // anchor point on the text bounding box
        128,                       // anchor x
        128,                       // anchor y
        2,                        // text size multiplier
        RIV_COLOR_LIGHTBLUE       // text color
    );

    // Make "press to start blink" by changing the color depending on the frame number
    uint32_t col = (riv->frame % 10 == 0) ? RIV_COLOR_LIGHTRED : RIV_COLOR_DARKRED;
    // Draw press to start
    riv_draw_text("PRESS ANY KEY TO START", RIV_SPRITESHEET_FONT_5X7, RIV_CENTER, 128, 128+16, 1, col);
}

// Draw the game map
void draw_game_screen() {
    draw_spaceship(player);
    draw_shots(*player.shots);

    draw_asteroids(*asteroids);

    // draw score
    char score_info[20];
    sprintf(score_info, "Score: %d", score);
    riv_draw_text(score_info, RIV_SPRITESHEET_FONT_3X5, RIV_TOPLEFT, 0, 5, 1, RIV_COLOR_WHITE);
}

void draw_game_over_screen() {
    // Draw last game frame
    draw_game_screen();
    // Draw GAME OVER
    riv_draw_text("GAME OVER", RIV_SPRITESHEET_FONT_5X7, RIV_CENTER, 128, 128, 2, RIV_COLOR_RED);
}


////////////
// update
////////////
void start_game() {
    riv_printf("GAME START\n");
    game_state = PLAYING;

    // initialize game variables
    player.direction = M_PI; // 90 degrees
    player.position = (riv_vec2f){MAP_SIZE / 2, MAP_SIZE / 2};
    player.size = TILE_SIZE;
    player.lifes = DEFAULT_PLAYER_LIFES;
    player.shots = initList();
    player.last_shot_frame = -1000;

    asteroids = initList();
    Asteroid asteroid;
    for (int i = 0; i < DEFAULT_INITIAL_ASTEROIDS; i++) {
        asteroid = spawn_asteroid();
        push(asteroids, &asteroid, sizeof(Asteroid));
    }
}

// Called when game ends
void end_game() {
    riv_printf("GAME OVER\n");
    game_state = ENDED;
    
    // free allocated memory
    clear(player.shots);
    clear(asteroids);

    // Quit in 3 seconds
    riv->quit_frame = riv->frame + 3*riv->target_fps;
}

void update_menu() {
    // Let game start whenever a key has been pressed
    if (riv->key_toggle_count > 0) {
        start_game();
    }
}

bool collided(Asteroid *asteroid, void *object, OBJECT type) {
    riv_vec2f obj_position;
    int obj_size;

    switch (type) {
        case SPACESHIP:
            Spaceship *spaceship = (Spaceship *)object;
            obj_position = spaceship->position;
            obj_size = spaceship->size;
            break;
        
        case SHOT:
            Shot *shot = (Shot *)object;
            obj_position = shot->position;
            obj_size = shot->size;
            break;

        case ASTEROID:
            Asteroid *asteroid = (Asteroid *)object;
            obj_position = asteroid->position;
            obj_size = ASTEROIDS_SIZES[asteroid->type];
            break;
    }

    int asteroid_size = ASTEROIDS_SIZES[asteroid->type];
    float asteroid_x = asteroid->position.x*TILE_SIZE;
    float asteroid_y = asteroid->position.y*TILE_SIZE;
    float obj_x = obj_position.x*TILE_SIZE;
    float obj_y = obj_position.y*TILE_SIZE;
    bool collided = false;

    // check if one of the extremes of object
    // is in contact with the asteroid
    // (x0,y0), (x1,y0), (x0, y1), (x1,y1)
    if (
        (
            obj_x >= asteroid_x &&
            obj_y >= asteroid_y &&
            obj_x <= asteroid_x + asteroid_size &&
            obj_y <= asteroid_y + asteroid_size
        ) ||
        (
            obj_x + obj_size >= asteroid_x &&
            obj_y >= asteroid_y &&
            obj_x + obj_size <= asteroid_x + asteroid_size &&
            obj_y <= asteroid_y + asteroid_size

        ) ||

        (
            obj_x >= asteroid_x &&
            obj_y + obj_size >= asteroid_y &&
            obj_x <= asteroid_x + asteroid_size &&
            obj_y + obj_size <= asteroid_y + asteroid_size

        ) ||

        (
            obj_x + obj_size >= asteroid_x &&
            obj_y + obj_size >= asteroid_y &&
            obj_x + obj_size <= asteroid_x + asteroid_size &&
            obj_y + obj_size <= asteroid_y + asteroid_size

        )
    ) {
        collided = true;
    }
    
    return collided;
}


// Update game logic
void update_game() {
    // update spaceship position
    update_spaceship(&player);

    // update shots positions
    update_shots(player.shots);

    // update asteroids positions and check collisions
    Node *node = asteroids->first;
    Asteroid *asteroid;
    int asteroid_index = 0;
    int shot_index;
    Node *node2;
    while (node) {
        asteroid = (Asteroid*)node->data;
        update_asteroid(asteroid);
        
        bool collision = false;

        collision = collided(asteroid, &player, SPACESHIP);
        if (collision) {
            player.lifes = player.lifes-1;
            if (player.lifes == 0) end_game();

            removeNode(asteroids, asteroid_index);
        }
        
        // check collision with a shot
        node2 = player.shots->first;
        shot_index = 0;
        while (!collision && node2) {
            Shot *shot = (Shot *)node2->data;
            collision = collided(asteroid, shot, SHOT);
            if (collision) {
                removeNode(asteroids, asteroid_index);
                removeNode(player.shots, shot_index);
                score += ASTEROIDS_SCORE[asteroid->type];
            }

            node2 = node2->next;
            shot_index++;
        }

        // check collision with another asteroid
        // node2 = node->next;
        // while (!collision && node2) {
        //     Asteroid *asteroid2 = (Asteroid*)node2->data;
        //     collision = collided(asteroid, asteroid2, ASTEROID);
        //     if (collision) {
        //         removeNode(asteroids, asteroid_index);
        //         // TODO: destroy asteroid2
        //         break;
        //     }

        //     node2 = node2->next;
        // }

        // next asteroid in the list
        node = node->next;
        if (!collision) asteroid_index++;
    }

    // spawn asteroid
    if (!(riv->frame - last_spawn_frame < spawn_cooldown*riv->target_fps)) {
        last_spawn_frame = riv->frame;
        
        Asteroid asteroid;
        if (asteroids->len == 0) {
            for (int i = 0; i < 3; i++) {
                asteroid = spawn_asteroid();
                push(asteroids, &asteroid, sizeof(Asteroid));
            }

        } else if (riv_rand_float() <= spawn_probability) {
            asteroid = spawn_asteroid();
            push(asteroids, &asteroid, sizeof(Asteroid));
        }
    }

    if (!(riv->frame - last_frame < probability_increase_cooldown*riv->target_fps)) {
        spawn_probability += 0.01;
        last_frame = riv->frame;
    }
}



int main() {
    riv->target_fps = 30;
    game_state = MENU;

    do {
        // Clear screen
        riv_clear(RIV_COLOR_DARKSLATE);

        switch(game_state) {
            case MENU:
                draw_menu_screen();
                update_menu();
                break;
            case PLAYING:
                draw_game_screen();
                update_game();
                break;
            case ENDED:
                draw_game_over_screen();
                break;
        }
    } while(riv_present());

    return 0;
}
