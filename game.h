#ifndef GAME_H
#define GAME_H

#define INITIAL_SPEED 2

#include <SDL.h>
#include <SDL_image.h>
#include "gameobjects.h"

struct Player {
    int x, y;
    int dx = 0, dy = 0;
    int acceleration = 2;
    int speed = INITIAL_SPEED;
    int maxspeed = 4;
    SDL_Rect rect;

    void move() {
        x += dx;
        y += dy;
        rect.x = x;
        rect.y = y;
    }
    void increaseSpeed() {
        if (speed<maxspeed){
            speed += acceleration;
        }
    }
    void checkincreaseSpeed(const Uint8* currentKeyStates) {
        if (currentKeyStates[SDL_SCANCODE_LCTRL] || currentKeyStates[SDL_SCANCODE_RCTRL]) {
            increaseSpeed();
        }
        else {
            speed = INITIAL_SPEED;
        }
    }

    bool isMoving() const{
        return dx !=0 || dy !=0;
    }

    bool isMovingNorth() const{
        return dy < 0;
    }
    bool isMovingSouth() const{
        return dy > 0;
    }
    bool isMovingWest() const{
        return dx < 0;
    }
    bool isMovingEast() const{
        return dx > 0;
    }

    void moveNorth() {
        dy = -speed;
        dx = 0;
    }
    void moveSouth() {
        dy = speed;
        dx = 0;
    }
    void moveWest() {
        dy = 0;
        dx = -speed;
    }
    void moveEast() {
        dy = 0;
        dx = speed;
    }
    void stopMovement() {
        dx = 0;
        dy = 0;
    }
};

void render(const Player& player, const Graphics& graphics)
{

    SDL_Texture* StandardplayerTex;

    SDL_Texture* MoveWestplayerTex;
    SDL_Texture* MovingWestplayerTex;

    SDL_Texture* MoveEastplayerTex;
    SDL_Texture* MovingEastplayerTex;

    SDL_Texture* MoveNorthplayerTex;
    SDL_Texture* MovingNorthplayerTex;

    SDL_Texture* MoveSouthplayerTex;
    SDL_Texture* MovingSouthplayerTex;

    StandardplayerTex = IMG_LoadTexture(graphics.renderer, "characters/standard.jpg");

    MoveNorthplayerTex = IMG_LoadTexture(graphics.renderer, "characters/movenorth.jpg");
    MovingNorthplayerTex = IMG_LoadTexture(graphics.renderer,"characters/movingnorth.jpg");

    MoveSouthplayerTex = IMG_LoadTexture(graphics.renderer, "characters/movesouth.jpg");
    MovingSouthplayerTex = IMG_LoadTexture(graphics.renderer, "characters/movingsouth.jpg");

    MoveWestplayerTex = IMG_LoadTexture(graphics.renderer, "characters/movewest.jpg");
    MovingWestplayerTex = IMG_LoadTexture(graphics.renderer,"characters/movingwest.jpg");

    MoveEastplayerTex = IMG_LoadTexture(graphics.renderer, "characters/moveeast.jpg");
    MovingEastplayerTex = IMG_LoadTexture(graphics.renderer,"characters/movingeast.jpg");

    SDL_Texture* CurrentplayerTex = StandardplayerTex;
    SDL_Texture* StoppingplayerTex = StandardplayerTex;

    const SDL_Rect clips[] = {
        {  0, 0, 64, 64},
        { 64, 0, 64, 64},
        {128, 0, 64, 64},
        {192, 0, 64, 64}
    };
    int CurrentFrame = 0;
//    if (player.isMoving()){

//        frameIndex = (SDL_GetTicks() / 100) % PLAYER_FRAMES;


//        sprite.tick();
//        const SDL_Rect* currentClip = sprite.getCurrentClip();
//        SDL_RenderCopy(graphics.renderer, sprite.spritetexture, currentClip, &destRect);
//        SDL_RenderPresent(graphics.renderer);
//    }

//    else{
    if (player.isMovingNorth()) {
        CurrentFrame = 1;
        CurrentplayerTex = MoveNorthplayerTex;
    }
    else if (player.isMovingSouth()) {
        //CurrentFrame = 2;
        CurrentplayerTex = MoveSouthplayerTex;
    }
    else if (player.isMovingWest()) {
        //CurrentFrame = 3;
        CurrentplayerTex = MoveWestplayerTex;
    }
    else if (player.isMovingEast()) {
        //CurrentFrame = 4;
        CurrentplayerTex = MoveEastplayerTex;
    }

    const SDL_Rect* CurrentClip = &clips[CurrentFrame];
    SDL_Rect destRect = {player.x, player.y, CurrentClip->w, CurrentClip->h };

    SDL_QueryTexture(CurrentplayerTex, NULL, NULL, &destRect.w, &destRect.h);
    SDL_RenderClear(graphics.renderer);
    SDL_RenderCopy(graphics.renderer, CurrentplayerTex, NULL, &destRect);
    SDL_RenderPresent(graphics.renderer);

    SDL_DestroyTexture(CurrentplayerTex);
}

bool gameOver(const Player& player) {
    return player.x < 0 || player.x >= SCREEN_WIDTH ||
           player.y < 0 || player.y >= SCREEN_HEIGHT;
}

#endif
