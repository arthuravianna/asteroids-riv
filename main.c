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
int ticks = 0;
int score = 0;
int last_spawn_frame = 0;
int last_frame = 0; // used to count time in seconds
int asteroids_destroyed[3] = {0, 0, 0};
game_params params = {
    .asteroids_initial_qty = DEFAULT_ASTEROIDS_INITIAL_QTY,
    .asteroids_spawn_prob = DEFAULT_ASTEROIDS_SPAWN_PROB,
    .asteroids_spawn_prob_increase_cooldown = DEFAULT_ASTEROIDS_SPAWN_PROB_INCREASE_COOLDOWN,
    .asteroids_spawn_cooldown = DEFAULT_ASTEROIDS_SPAWN_COOLDOWN,
    .player_lifes = DEFAULT_PLAYER_LIFES,
    .player_acceleration = DEFAULT_PLAYER_ACCELERATION,
    .player_shot_speed = DEFAULT_SHOT_SPEED,
    .player_shot_cooldown = DEFAULT_SHOT_COOLDOWN,
    .player_collision_cooldown = DEFAULT_PLAYER_COLLISION_COOLDOWN
};


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


void draw_game_screen() {
    draw_spaceship(player);
    draw_shots(*player.shots);

    draw_asteroids(*asteroids);

    // draw player pannel score
    char score_info[20];
    sprintf(score_info, "Score: %d", score);
    riv_draw_text(score_info, RIV_SPRITESHEET_FONT_3X5, RIV_TOPLEFT, 5, 5, 1, RIV_COLOR_WHITE);

    int x;
    for (int i = 0; i < player.lifes; i++) {
        x = 8 + (8*i);
        riv_draw_triangle_line(x, 12, x - 3, 18, x + 3, 18, RIV_COLOR_WHITE);
    }
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
    player.direction = 0;
    player.position = (riv_vec2f){MAP_SIZE / 2, MAP_SIZE / 2};
    player.size = TILE_SIZE;
    player.shots = initList();
    player.last_shot_frame = -1000;
    player.collision_frame = 0;
    player.lifes = params.player_lifes;
    player.acceleration = params.player_acceleration;
    player.shot_speed = params.player_shot_speed;
    player.shot_cooldown = params.player_shot_cooldown;
    player.collision_cooldown = params.player_collision_cooldown;

    asteroids = initList();
    Asteroid asteroid;
    for (int i = 0; i < params.asteroids_initial_qty; i++) {
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
    float asteroid_x = asteroid->position.x;
    float asteroid_y = asteroid->position.y;
    float obj_x = obj_position.x;
    float obj_y = obj_position.y;
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
    ticks++;
    // update spaceship position
    update_spaceship(&player);

    // update shots positions
    update_shots(player.shots);

    // update asteroids positions and check collisions
    Node *node = asteroids->first;
    Asteroid *asteroid;
    int asteroid_index = 0;
    int asteroid_type;
    int shot_index;
    Node *node2;
    while (node) {
        asteroid = (Asteroid*)node->data;
        asteroid_type = asteroid->type;
        update_asteroid(asteroid);
        
        bool collision = false;

        if (!collision_cooldown(player)) {
            collision = collided(asteroid, &player, SPACESHIP);
            if (collision) {
                player.collision_frame = riv->frame;

                player.lifes = player.lifes-1;
                if (player.lifes == 0) end_game();

                removeNode(asteroids, asteroid_index);
            }
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

                asteroids_destroyed[asteroid_type]++;
            }

            node2 = node2->next;
            shot_index++;
        }

        // next asteroid in the list
        node = node->next;
        if (!collision) {
            asteroid_index++;
        } else {
            int children_asteroid_type = -1;
            if (asteroid->type == BIG_ASTEROID) {
                children_asteroid_type = MEDIUM_ASTEROID;
            } else if (asteroid->type == MEDIUM_ASTEROID) {
                children_asteroid_type = SMALL_ASTEROID;
            }

            if (children_asteroid_type != -1) {
                int size = ASTEROIDS_SIZES[children_asteroid_type];
                Asteroid child_asteroid = {
                    .position = (riv_vec2f){asteroid->position.x + size, asteroid->position.y},
                    .direction = asteroid->direction,
                    .type = children_asteroid_type,
                    .format = asteroid->format,
                };

                push(asteroids, &child_asteroid, sizeof(Asteroid));
                child_asteroid.position = (riv_vec2f){asteroid->position.x - size, asteroid->position.y};
                push(asteroids, &child_asteroid, sizeof(Asteroid));
            }
        }
    }

    // spawn asteroid
    if (!(riv->frame - last_spawn_frame < params.asteroids_spawn_cooldown*riv->target_fps)) {
        last_spawn_frame = riv->frame;
        
        Asteroid asteroid;
        if (asteroids->len == 0) {
            for (int i = 0; i < 3; i++) {
                asteroid = spawn_asteroid();
                push(asteroids, &asteroid, sizeof(Asteroid));
            }

        } else if (riv_rand_float() <= params.asteroids_spawn_prob) {
            asteroid = spawn_asteroid();
            push(asteroids, &asteroid, sizeof(Asteroid));
        }
    }

    if (!(riv->frame - last_frame < params.asteroids_spawn_prob_increase_cooldown*riv->target_fps)) {
        params.asteroids_spawn_prob += 0.01;
        last_frame = riv->frame;
    }
}

void update_scores() {
    riv->outcard_len = riv_snprintf(
        (char*)riv->outcard,
        RIV_SIZE_OUTCARD,
        "JSON{\"score\":%d, \"lifes\":%d, \"asteroids_bg\":%d, \"asteroids_md\":%d, \"asteroids_sm\":%d}",
        score, player.lifes, asteroids_destroyed[BIG_ASTEROID], asteroids_destroyed[MEDIUM_ASTEROID], asteroids_destroyed[SMALL_ASTEROID]
    );
}


int main(int argc, char* argv[]) {
     if (argc > 1) {
        if (argc % 2 == 0) {
            riv_printf("Wrong number of arguments\n");
            return 1;
        }

        for (int i = 1; i < argc; i+=2) {
            if (strcmp(argv[i], "-asteroids-initial-qty") == 0) {
                params.asteroids_initial_qty = atoi(argv[i+1]);
            } else if (strcmp(argv[i], "-asteroids-spawn-prob") == 0) {
                params.asteroids_spawn_prob = atof(argv[i+1]);
            } else if (strcmp(argv[i], "-asteroids-spawn-prob-increase-cooldown") == 0) {
                params.asteroids_spawn_prob_increase_cooldown = atof(argv[i+1]);
            } else if (strcmp(argv[i], "-asteroids-spawn-cooldown") == 0) {
                params.asteroids_spawn_cooldown = atof(argv[i+1]);
            } else if (strcmp(argv[i], "-player-lifes") == 0) {
                params.player_lifes = atoi(argv[i+1]);
            } else if (strcmp(argv[i], "-player-shot-cooldown") == 0) {
                params.player_shot_cooldown = atof(argv[i+1]);
            } else if (strcmp(argv[i], "-player-shot-speed") == 0) {
                params.player_shot_speed = atof(argv[i+1]);
            } else if (strcmp(argv[i], "-player-acceleration") == 0) {
                params.player_acceleration = atof(argv[i+1]);
            } else if (strcmp(argv[i], "-player-collision-cooldown") == 0) {
                params.player_collision_cooldown = atof(argv[i+1]);
            }
        }
    }

    riv_printf("### PARAMS ###\n\
        asteroids initial qty: %d \n\
        asteroids spawn prob: %f \n\
        asteroids spawn prob. inc. cooldown: %f \n\
        asteroids spawn cooldown: %f \n\
        player lifes: %d \n\
        player shot cooldown: %f \n\
        player shot speed: %f \n\
        player acc: %f \n\
        player collision cooldown: %f\n\n",
        params.asteroids_initial_qty, params.asteroids_spawn_prob,
        params.asteroids_spawn_prob_increase_cooldown, params.asteroids_spawn_cooldown,
        params.player_lifes, params.player_shot_cooldown,
        params.player_shot_speed, params.player_acceleration,
        params.player_collision_cooldown
    );

    riv->target_fps = 30;
    game_state = MENU;

    do {
        // Clear screen
        riv_clear(RIV_COLOR_BLACK);

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
        
        update_scores();
    } while(riv_present());

    return 0;
}
