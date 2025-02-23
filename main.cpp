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

    Player player;
    player.x = 10;
    player.y = 540;
    player.rect = {player.x, player.y, 64, 64};

    Sprite sprite;
    SDL_Texture* playerTexture = graphics.loadTexture(PLAYER_SPRITE_FILE);
    sprite.init(playerTexture, PLAYER_FRAMES, clips);

    //ScrollingBackground background;
    //background.setTexture(graphics.loadTexture("backgrounds/123456.png"));

    //Obstacle obstacles;
    initObstacles(graphics);

    graphics.presentScene();
    waitUntilKeyPressed();

    bool quit = false;
    SDL_Event event;
    while (!quit && !gameOver(player)) {
        Uint32 currentTime = SDL_GetTicks();
        graphics.prepareScene();

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) quit = true;
        }

        sprite.tick(currentTime);

        graphics.rendersprite(player.x, player.y, sprite);

        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

        player.checkincreaseSpeed(currentKeyStates);

        if (currentKeyStates[SDL_SCANCODE_UP]) {
            player.moveNorth();
        }
        else if (currentKeyStates[SDL_SCANCODE_DOWN]) {
            player.moveSouth();
        }
        else if (currentKeyStates[SDL_SCANCODE_LEFT]) {
            player.moveWest();
        }
        else if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
            player.moveEast();
        }
        else {
            player.stopMovement();
        }

        player.move();
        checkCollisions(player, obstacles);

        render(player, graphics);
        renderObstacles(graphics);

        //background.scroll(1);
        //graphics.renderbackground(background);

        graphics.presentScene();
        SDL_Delay(1);

    }
    //SDL_DestroyTexture( playerTexture );
    //playerTexture = nullptr;
    //SDL_DestroyTexture( background.texture );
    graphics.quit();

    return 0;
}
