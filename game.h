#ifndef game_settings
#define game_settings
    #define MAP_SIZE 32
    #define TILE_SIZE 8
    #define DEFAULT_PLAYER_LIFES 3
    #define DEFAULT_INITIAL_ASTEROIDS 5

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
