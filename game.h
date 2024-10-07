#ifndef game_settings
#define game_settings
    #define MAP_SIZE 256
    #define TILE_SIZE 8
    #define DEFAULT_ASTEROIDS_INITIAL_QTY 5
    #define DEFAULT_ASTEROIDS_SPAWN_PROB 0.05
    #define DEFAULT_ASTEROIDS_SPAWN_PROB_INCREASE_COOLDOWN 5
    #define DEFAULT_ASTEROIDS_SPAWN_COOLDOWN 2
    #define DEFAULT_PLAYER_LIFES 3
    #define DEFAULT_SHOT_COOLDOWN 0.3
    #define DEFAULT_SHOT_SPEED 6
    #define DEFAULT_PLAYER_ACCELERATION 0.05 
    #define DEFAULT_PLAYER_COLLISION_COOLDOWN 3

    typedef struct game_params {
        int asteroids_initial_qty;
        float asteroids_spawn_prob;                   // prob. of spawning an asteroid.
        float asteroids_spawn_prob_increase_cooldown; // cooldown to increase prob. of spawning an asteroid (in seconds).
        float asteroids_spawn_cooldown;               // cooldown to spawn a new asteroid.
        int player_lifes;                             // quantity of player lifes
        float player_shot_cooldown;                   // player shot cooldown (in seconds).
        float player_shot_speed;                      // speed of a player's shot.
        float player_acceleration;                    // player's spaceship acceleration.
        float player_collision_cooldown;              // player's ghostmode duration after being hit by an asteroid.
    } game_params;

    typedef struct object_sizes {
        float width;
        float height;
    } Size;

    typedef enum game_objects {
        SPACESHIP,
        SHOT,
        ASTEROID
    } OBJECT;
#endif
