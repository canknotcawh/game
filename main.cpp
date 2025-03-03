#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "graphics.h"
#include "defs.h"
#include "game.h"
#include "gameobjects.h"
#include "buttons.h"
#include "menu.h"

using namespace std;

int main(int argc, char* argv[])
{
    Graphics graphics;
    graphics.init();

    SDL_Renderer* renderer = graphics.getRenderer();
    ButtonManager buttonManager(renderer);

    CTX ctx;
    ctx.window = graphics.getWindow();
    ctx.renderer = graphics.getRenderer();

    bool gameRunning = showMenu(renderer, buttonManager);
    if (!gameRunning) {
        graphics.quit();
        return 0;
    }

    SDL_Texture* playerTexture = graphics.loadTexture(PLAYER_SPRITE_FILE);
    ctx.player.sprite.init(playerTexture, PLAYER_FRAMES, player_movingsouth_clips);

    bool quit = false;
    bool pause = false;
    SDL_Event event;

    while (!quit && !gameOver(ctx.player)) {
        Uint32 currentTime = SDL_GetTicks();
        graphics.prepareScene();

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) quit = true;

            int x, y;
            SDL_GetMouseState(&x, &y);

            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (buttonManager.pausebutton.isClicked(x, y)) {
                    pause = !pause;
                }
            }
        }

        if (pause) {
            int pauseAction = showPauseMenu(renderer, buttonManager);
            if (pauseAction == 0) {
                quit = true;
            }
            else if (pauseAction == -1) {
                gameRunning = showMenu(renderer, buttonManager);
                if (!gameRunning) {
                    graphics.quit();
                    return 0;
                }
            }
            pause = false;
        }

        if (!pause) {
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
    }

    buttonManager.pausebutton.render(renderer);
    graphics.presentScene();
    SDL_Delay(16);
}

    graphics.quit();
    return 0;
}
