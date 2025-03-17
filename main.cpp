#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "graphics.h"
#include "defs.h"
#include "game.h"
#include "button.h"
#include "menu.h"

using namespace std;

int main(int argc, char* argv[])
{
    Graphics graphics;
    graphics.init();

    SDL_Renderer* renderer = graphics.getRenderer();
    ButtonManager buttonManager(renderer);

    SDL_Texture* backgroundTexture = graphics.loadTexture(BACKGROUND_FILE);

    if (!backgroundTexture) {
        SDL_Log("Failed to load background texture: %s", IMG_GetError());
    } else {
        int w, h;
        SDL_QueryTexture(backgroundTexture, NULL, NULL, &w, &h);
        SDL_Log("Background loaded: Width = %d, Height = %d", w, h);
    }

    CTX ctx;
    ctx.window = graphics.getWindow();
    ctx.renderer = graphics.getRenderer();

    bool gameRunning = showMenu(renderer, buttonManager);
    if (!gameRunning) {
        graphics.quit();
        return 0;
    }

    SDL_Texture* playerTexture = graphics.loadTexture(PLAYER_SPRITE_FILE);
    ctx.player.sprite.init(playerTexture, PLAYER_FRAMES, player_idle_clips);

    bool quit = false;
    SDL_Event event;

    while (!quit && !gameOver(ctx.player)) {
        Uint32 currentTime = SDL_GetTicks();

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) quit = true;
            handleEvent(event, ctx);
        }

        handleMovement(ctx, currentTime);
        ctx.player.move();

        graphics.prepareScene(backgroundTexture);
        render(ctx);
        graphics.presentScene();

        SDL_Delay(16);
    }

    SDL_DestroyTexture(backgroundTexture);
    graphics.quit();
    return 0;
}
