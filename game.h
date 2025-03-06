#ifndef GAME_H
#define GAME_H

#define INITIAL_SPEED 2

#include <SDL.h>
#include <SDL_image.h>
#include "graphics.h"
#include "gameobjects.h"

struct Player {
    int x, y;
    int dx = 0, dy = 0;
    int speed = INITIAL_SPEED;
    SDL_Rect rect;
    Sprite sprite;

    int direction = 0;

    void setDirection(int direction, SDL_Texture* texture) {
    if (this->direction != direction) {
        this->direction = direction;
        sprite.currentFrame = 0;
        sprite.lastTickTime = SDL_GetTicks();

        switch (direction) {
            case 0:{
                sprite.init(texture, PLAYER_FRAMES, player_movingsouth_clips);
                break;
            }
            case 1:{
                sprite.init(texture, PLAYER_FRAMES, player_movingnorth_clips);
                break;
            }
            case 2:{
                sprite.init(texture, PLAYER_FRAMES, player_movingwest_clips);
                break;
            }
            case 3:{
                sprite.init(texture, PLAYER_FRAMES, player_movingeast_clips);
                break;
            }
        }
    }
}

    Player(){
        x = 20;
        y = 500;
        rect = { x, y, PLAYER_WIDTH, PLAYER_HEIGHT };
    }

    void move() {
        x += dx;
        y += dy;
        rect.x = x;
        rect.y = y;
    }

    void moveSouth() {
        dy = speed;
        dx = 0;
    }
    void moveNorth() {
        dy = -speed;
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
        sprite.currentFrame = 0;
    }

    bool isMoving() const{
        return dx != 0 || dy !=0;
    }
    bool isMovingSouth() const{
        return dy > 0;
    }
    bool isMovingNorth() const{
        return dy < 0;
    }
    bool isMovingWest() const{
        return dx < 0;
    }
    bool isMovingEast() const{
        return dx > 0;
    }
};

struct Background {
    SDL_Texture* texture;
    int SCREEN_WIDTH;
    int SCREEN_HEIGHT;

    void init(SDL_Texture* tex, int width, int height) {
        texture = tex;
        SCREEN_WIDTH = width;
        SCREEN_HEIGHT = height;
    }

    void render(SDL_Renderer* renderer) {
        SDL_Rect bgRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
        SDL_RenderCopy(renderer, texture, NULL, &bgRect);
    }
};


struct CTX {

    SDL_Window* window;
    SDL_Renderer* renderer;
    Player player;
    Background background;
};

inline void initClips(CTX& ctx) {
    SDL_Texture* texture = IMG_LoadTexture(ctx.renderer, PLAYER_SPRITE_FILE);
    ctx.player.sprite.texture = texture;
    ctx.player.sprite.init(texture, PLAYER_FRAMES, player_movingsouth_clips);
}

inline void handleMovement(CTX& ctx, Uint32 currentTime) {
    Player& player = ctx.player;
    if (player.isMovingSouth()){
        player.setDirection(0, player.sprite.texture);
    }
    else if (player.isMovingNorth()){
        player.setDirection(1, player.sprite.texture);
    }
    else if (player.isMovingWest()){
        player.setDirection(2, player.sprite.texture);
    }
    else if (player.isMovingEast()){
        player.setDirection(3, player.sprite.texture);
    }

    if (player.isMoving()) {
        player.sprite.tick(currentTime);
        player.move();
    }
}

inline void render(CTX& ctx) {
    SDL_RenderClear(ctx.renderer);

    Sprite& sprite = ctx.player.sprite;

    const SDL_Rect* currentClip = &ctx.player.sprite.clips[ctx.player.sprite.currentFrame];

    SDL_Rect destRect = { ctx.player.x, ctx.player.y, currentClip->w, currentClip->h };

    SDL_RenderCopy(ctx.renderer, ctx.player.sprite.texture, currentClip, &destRect);
    SDL_RenderPresent(ctx.renderer);
}

inline bool gameOver(const Player& player) {
    return player.x < 0 || player.x >= SCREEN_WIDTH ||
           player.y < 0 || player.y >= SCREEN_HEIGHT;
}
#endif // GAME_H
