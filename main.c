#include <math.h>
#include <riv.h>
#include "game.h"
//#include "asteroids.h"
#include "spaceship.h"

enum GAME_STATE {
    MENU,
    PLAYING,
    ENDED
};

enum GAME_STATE game_state;

Spaceship player;
//Asteroids asteroids;

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
    // TODO: draw game
    draw_spaceship(player);
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
    player.lifes = DEFAULT_PLAYER_LIFES;
    player.shots = initList();
}

// Called when game ends
void end_game() {
    riv_printf("GAME OVER\n");
    game_state = ENDED;
    
    // free allocated memory
    clear(player.shots);

    // Quit in 3 seconds
    riv->quit_frame = riv->frame + 3*riv->target_fps;
}

void update_menu() {
    // Let game start whenever a key has been pressed
    if (riv->key_toggle_count > 0) {
        start_game();
    }
}

// Update game logic
void update_game() {
    update_spaceship(&player);

    //end_game();
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
