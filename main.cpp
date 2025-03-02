#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "graphics.h"
#include "defs.h"
#include "game.h"
#include "gameobjects.h"

using namespace std;

void waitUntilKeyPressed()
{
    SDL_Event e;
    while (true) {
        if ( SDL_PollEvent(&e) != 0 &&
             (e.type == SDL_KEYDOWN || e.type == SDL_QUIT) )
            return;
        SDL_Delay(10);
    }
}

int main(int argc, char* argv[])
{

    Graphics graphics;
    graphics.init();

    CTX ctx;
    ctx.window = graphics.getWindow();
    ctx.renderer = graphics.getRenderer();

    SDL_Texture* playerTexture = graphics.loadTexture(PLAYER_SPRITE_FILE);
    if (!playerTexture) {
        cerr << "Error loading player sprite!\n";
        return -1;
    }

    ctx.player.sprite.init(playerTexture, PLAYER_FRAMES, player_movingsouth_clips);

    graphics.presentScene();
    waitUntilKeyPressed();

    bool quit = false;
    SDL_Event event;

    while (!quit && !gameOver(ctx.player)) {
        Uint32 currentTime = SDL_GetTicks();
        graphics.prepareScene();

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) quit = true;
        }

        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

        if (currentKeyStates[SDL_SCANCODE_UP]) {
            ctx.player.moveNorth();
            ctx.player.sprite.clips.assign(std::begin(player_movingnorth_clips), std::end(player_movingnorth_clips));
        }
        else if (currentKeyStates[SDL_SCANCODE_DOWN]) {
            ctx.player.moveSouth();
            ctx.player.sprite.clips.assign(std::begin(player_movingsouth_clips), std::end(player_movingsouth_clips));
        }
        else if (currentKeyStates[SDL_SCANCODE_LEFT]) {
            ctx.player.moveWest();
            ctx.player.sprite.clips.assign(std::begin(player_movingwest_clips), std::end(player_movingwest_clips));
        }
        else if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
            ctx.player.moveEast();
            ctx.player.sprite.clips.assign(std::begin(player_movingeast_clips), std::end(player_movingeast_clips));
        }
        else {
            ctx.player.stopMovement();
        }

        handleMovement(ctx, currentTime);
        ctx.player.move();

        render(ctx);

        graphics.presentScene();
        SDL_Delay(16);

    }
    graphics.quit();

    return 0;
}
