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

    SDL_Texture* bgTexture = graphics.loadTexture("backgrounds/background2.png");
    if (!bgTexture) {
        std::cerr << "ERROR SDL: " << IMG_GetError() << std::endl;
    }

    bool gameRunning = showMenu(renderer, buttonManager);
    if (!gameRunning) {
        graphics.quit();
        return 0;
    }

    ctx.background.init(bgTexture, SCREEN_WIDTH, SCREEN_HEIGHT);

    SDL_Texture* playerTexture = graphics.loadTexture(PLAYER_SPRITE_FILE);
    ctx.player.sprite.init(playerTexture, PLAYER_FRAMES, player_movingsouth_clips);

    bool quit = false;
    SDL_Event event;

    int bgX = 0;
    int imgWidth = 0, imgHeight = 0;
    SDL_QueryTexture(bgTexture, NULL, NULL, &imgWidth, &imgHeight);

    while (!quit && !gameOver(ctx.player)) {
        Uint32 currentTime = SDL_GetTicks();
        graphics.prepareScene(bgTexture);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) quit = true;
        }

        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

        if (currentKeyStates[SDL_SCANCODE_UP] || currentKeyStates[SDL_SCANCODE_W]) {
            ctx.player.moveNorth();
            ctx.player.sprite.clips.assign(std::begin(player_movingnorth_clips), std::end(player_movingnorth_clips));
        }
        else if (currentKeyStates[SDL_SCANCODE_DOWN] || currentKeyStates[SDL_SCANCODE_S]) {
            ctx.player.moveSouth();
            ctx.player.sprite.clips.assign(std::begin(player_movingsouth_clips), std::end(player_movingsouth_clips));
        }
        else if (currentKeyStates[SDL_SCANCODE_LEFT] || currentKeyStates[SDL_SCANCODE_A]) {
            ctx.player.moveWest();
            bgX += ctx.player.speed;
            ctx.player.sprite.clips.assign(std::begin(player_movingwest_clips), std::end(player_movingwest_clips));
        }
        else if (currentKeyStates[SDL_SCANCODE_RIGHT] || currentKeyStates[SDL_SCANCODE_D]) {
            ctx.player.moveEast();
            bgX -= ctx.player.speed;
            ctx.player.sprite.clips.assign(std::begin(player_movingeast_clips), std::end(player_movingeast_clips));
        }
        else {
            ctx.player.stopMovement();
        }

        handleMovement(ctx, currentTime);
        ctx.player.move();
        ctx.background.render(ctx.renderer);
        render(ctx);
        graphics.presentScene();
        SDL_Delay(16);
    }

    graphics.quit();
    return 0;
}
