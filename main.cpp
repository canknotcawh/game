#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include "graphics.h"
#include "defs.h"
#include "game.h"
#include "enemy.h"
#include "button.h"
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
    ctx.backgroundTexture = graphics.loadTexture(BACKGROUND_FILE);

    Level level;

    bool gameRunning = showMenu(renderer, buttonManager);
    if (!gameRunning) {
        graphics.quit();
        return 0;
    }

    SDL_Texture* playerTexture = graphics.loadTexture(PLAYER_SPRITE_FILE);
    ctx.player.sprite.init(playerTexture, PLAYER_FRAMES, player_idle_clips);
    SDL_Texture* enemyTexture = graphics.loadTexture(ENEMY_SPRITE_FILE);

    for (int i = 0; i < 3; i++) {
        spawnEnemy(ctx);
    }

    bool quit = false;
    SDL_Event event;

    while (!quit && !gameOver(ctx.player)) {
        Uint32 currentTime = SDL_GetTicks();

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) quit = true;
            handleEvent(event, ctx);
        }

        updateGame(ctx, currentTime);

        graphics.prepareScene();
        render(ctx);
        graphics.presentScene();

        SDL_Delay(16);
    }

    cleanup(ctx);
    graphics.quit();
    return 0;
}
