#ifndef GAME_H
#define GAME_H

#define INITIAL_SPEED 2

#include <SDL.h>
#include <SDL_image.h>
#include "graphics.h"

struct Player {
    int x, y;
    int dx = 0, dy = 0;
    int miny = SCREEN_HEIGHT / 2;
    int maxy = SCREEN_HEIGHT - PLAYER_HEIGHT;
    int speed = INITIAL_SPEED;
    SDL_Rect rect;
    Sprite sprite;
    SDL_Texture*attackingTexture = nullptr;

    Uint32 currentTime = SDL_GetTicks();
    int direction = 0;
    int lastDirection = 0;

    bool isRunning = false;
    bool isAttacking = false;
    Uint32 attackingStartTime = 0;

    void setDirection(int newDirection, SDL_Texture* texture) {
        if (direction != newDirection) {
            direction = newDirection;
            sprite.currentFrame = 0;
            sprite.lastTickTime = SDL_GetTicks();

            switch (direction) {
                case 0:
                    sprite.init(texture, PLAYER_FRAMES, player_movingsouth_clips);
                    break;
                case 1:
                    sprite.init(texture, PLAYER_FRAMES, player_movingnorth_clips);
                    break;
                case 2:
                    sprite.init(texture, PLAYER_FRAMES, player_movingwest_clips);
                    break;
                case 3:
                    sprite.init(texture, PLAYER_FRAMES, player_movingeast_clips);
                    break;
            }
        }
    }

    Player(){
        x = 20;
        y = SCREEN_HEIGHT - PLAYER_HEIGHT - 10;
        rect = { x, y, PLAYER_WIDTH, PLAYER_HEIGHT };
    }

    void move() {
        x += dx;
        y += dy;

        if (y < miny) y = miny;
        if (y > maxy) y = maxy;

        rect.x = x;
        rect.y = y;
    }

    void moveSouth() {
        dy = speed;
        dx = 0;
        direction = 0;
    }
    void moveNorth() {
        dy = -speed;
        dx = 0;
        direction = 1;
    }
    void moveWest() {
        dy = 0;
        dx = -speed;
        direction = 2;
    }
    void moveEast() {
        dy = 0;
        dx = speed;
        direction = 3;
    }
    void stopMovement() {
        dx = 0;
        dy = 0;
        sprite.currentFrame = 0;
    }

    void startAttacking(SDL_Texture* playerTexture) {
        if (!isAttacking && !isMoving()) {
            isAttacking = true;
            attackingStartTime = SDL_GetTicks();
            sprite.frameDelay = 80;

            lastDirection = direction;

            switch (direction) {
                case 0:
                    sprite.clips.assign(std::begin(player_idlepunch_south_clips), std::end(player_idlepunch_south_clips));
                    break;
                case 1:
                    sprite.clips.assign(std::begin(player_idlepunch_north_clips), std::end(player_idlepunch_north_clips));
                    break;
                case 2:
                    sprite.clips.assign(std::begin(player_idlepunch_west_clips), std::end(player_idlepunch_west_clips));
                    break;
                case 3:
                    sprite.clips.assign(std::begin(player_idlepunch_east_clips), std::end(player_idlepunch_east_clips));
                    break;
            }
            sprite.currentFrame = 0;
            sprite.lastTickTime = SDL_GetTicks();
        }
    }

    void updateAnimation(Uint32 currentTime) {
        if (isAttacking) {
            Uint32 elapsed = SDL_GetTicks() - attackingStartTime;
            int frameIndex = (elapsed / sprite.frameDelay);

            if (frameIndex < sprite.clips.size()) {
                sprite.currentFrame = frameIndex;
                sprite.tick(currentTime);
            }
            else {
                isAttacking = false;

                const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
                bool moveKeyPressed = currentKeyStates[SDL_SCANCODE_W] || currentKeyStates[SDL_SCANCODE_A] ||
                                      currentKeyStates[SDL_SCANCODE_S] || currentKeyStates[SDL_SCANCODE_D];

                if (moveKeyPressed) {
                    if (currentKeyStates[SDL_SCANCODE_W]) {
                        moveNorth();
                    }
                    else if (currentKeyStates[SDL_SCANCODE_S]) {
                        moveSouth();
                    }
                    else if (currentKeyStates[SDL_SCANCODE_A]) {
                        moveWest();
                    }
                    else if (currentKeyStates[SDL_SCANCODE_D]) {
                        moveEast();
                    }

                    switch (direction) {
                        case 0:
                            sprite.clips.assign(std::begin(player_movingsouth_clips), std::end(player_movingsouth_clips));
                            break;
                        case 1:
                            sprite.clips.assign(std::begin(player_movingnorth_clips), std::end(player_movingnorth_clips));
                            break;
                        case 2:
                            sprite.clips.assign(std::begin(player_movingwest_clips), std::end(player_movingwest_clips));
                            break;
                        case 3:
                            sprite.clips.assign(std::begin(player_movingeast_clips), std::end(player_movingeast_clips));
                            break;
                    }
                    sprite.currentFrame = 0;
                    sprite.lastTickTime = SDL_GetTicks();
                }
                else {
                    sprite.clips = { player_idle_clips[lastDirection] };
                }
            }
        }
    }

    bool isMoving() const{
        return dx != 0 || dy !=0;
    }
};

struct CTX {
    SDL_Window* window;
    SDL_Renderer* renderer;
    Player player;
};

inline void handleEvent(SDL_Event& event, CTX& ctx) {
    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
    SDL_Texture* playerTexture = ctx.player.sprite.texture;

    if (!ctx.player.isAttacking) {
        if (currentKeyStates[SDL_SCANCODE_W]) {
            ctx.player.moveNorth();
            ctx.player.sprite.clips.assign(std::begin(player_movingnorth_clips), std::end(player_movingnorth_clips));
        }
        else if (currentKeyStates[SDL_SCANCODE_S]) {
            ctx.player.moveSouth();
            ctx.player.sprite.clips.assign(std::begin(player_movingsouth_clips), std::end(player_movingsouth_clips));
        }
        else if (currentKeyStates[SDL_SCANCODE_A]) {
            ctx.player.moveWest();
            ctx.player.sprite.clips.assign(std::begin(player_movingwest_clips), std::end(player_movingwest_clips));
        }
        else if (currentKeyStates[SDL_SCANCODE_D]) {
            ctx.player.moveEast();
            ctx.player.sprite.clips.assign(std::begin(player_movingeast_clips), std::end(player_movingeast_clips));
        }
        else {
            ctx.player.stopMovement();
        }
    }

    if (currentKeyStates[SDL_SCANCODE_J]) {
        ctx.player.startAttacking(playerTexture);
    }
}

inline void initClips(CTX& ctx) {
    SDL_Texture* texture = IMG_LoadTexture(ctx.renderer, PLAYER_SPRITE_FILE);
    ctx.player.sprite.texture = texture;
    ctx.player.sprite.init(texture, PLAYER_FRAMES, player_idle_clips);
}

inline void handleMovement(CTX& ctx, Uint32 currentTime) {
    Player& player = ctx.player;
    player.updateAnimation(currentTime);

    if (!player.isAttacking) {
         if (player.isMoving()) {
            int newDirection = (player.dy > 0) ? 0 : (player.dy < 0) ? 1 : (player.dx < 0) ? 2 : 3;
            if (player.direction != newDirection) {
                player.setDirection(newDirection, player.sprite.texture);
                player.sprite.currentFrame = 0;
            }
            player.sprite.tick(currentTime);

            int newy = player.y + player.dy;
            if (newy >= player.miny && newy <= player.maxy) {
                player.move();
            }
        }
        else {
            player.sprite.init(player.sprite.texture, PLAYER_FRAMES, &player_idle_clips[player.direction]);
            player.sprite.currentFrame = 0;
        }
    }
}

inline void render(CTX& ctx) {
    Sprite& sprite = ctx.player.sprite;
    const SDL_Rect* currentClip = &ctx.player.sprite.clips[ctx.player.sprite.currentFrame];
    SDL_Rect destRect = { ctx.player.x - (currentClip->w - PLAYER_WIDTH) / 2, ctx.player.y, currentClip->w, currentClip->h };
    SDL_RenderCopy(ctx.renderer, ctx.player.sprite.texture, currentClip, &destRect);
    SDL_RenderPresent(ctx.renderer);
}

inline bool gameOver(const Player& player) {
    return player.x < 0 || player.x >= SCREEN_WIDTH ||
           player.y < 0 || player.y >= SCREEN_HEIGHT;
}
#endif // GAME_H
