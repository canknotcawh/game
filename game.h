#ifndef GAME_H
#define GAME_H

#define INITIAL_SPEED 2

#include <SDL.h>
#include <SDL_image.h>
#include "graphics.h"
#include "defs.h"
#include "enemy.h"

struct Player {
    int x, y;
    int dx = 0, dy = 0;
    int speed = INITIAL_SPEED;
    int minx = 0;
    int maxx = SCREEN_WIDTH - PLAYER_WIDTH;
    int miny = SCREEN_HEIGHT / 2 - 40;
    int maxy = SCREEN_HEIGHT - PLAYER_HEIGHT;

    int health = 100;
    Uint32 lastHealthDecrease = 0;
    int healDecreaseRate = 1000;

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
        x = SCREEN_WIDTH / 2 - PLAYER_WIDTH / 2;
        y = SCREEN_HEIGHT / 2 - PLAYER_HEIGHT;
        rect = { x, y, PLAYER_WIDTH, PLAYER_HEIGHT };
    }

    void move() {
        x += dx;
        y += dy;
        if (x < minx) x = minx;
        if (x > maxx) x = maxx;
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
        if (!isAttacking) {
            isAttacking = true;
            attackingStartTime = SDL_GetTicks();
            sprite.frameDelay = 100;

            lastDirection = direction;

            speed = INITIAL_SPEED / 2;

            const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
            bool moveKeyPressed = currentKeyStates[SDL_SCANCODE_W] || currentKeyStates[SDL_SCANCODE_A] || currentKeyStates[SDL_SCANCODE_S] || currentKeyStates[SDL_SCANCODE_D];

            if (!moveKeyPressed) {
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
            }

            else {
                switch (direction) {
                    case 0:
                        sprite.clips.assign(std::begin(player_movingpunch_south_clips), std::end(player_movingpunch_south_clips));
                        break;
                    case 1:
                        sprite.clips.assign(std::begin(player_movingpunch_north_clips), std::end(player_movingpunch_north_clips));
                        break;
                    case 2:
                        sprite.clips.assign(std::begin(player_movingpunch_west_clips), std::end(player_movingpunch_west_clips));
                        break;
                    case 3:
                        sprite.clips.assign(std::begin(player_movingpunch_east_clips), std::end(player_movingpunch_east_clips));
                        break;
                }
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

                speed = INITIAL_SPEED;

                const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
                bool moveKeyPressed = currentKeyStates[SDL_SCANCODE_W] || currentKeyStates[SDL_SCANCODE_A] || currentKeyStates[SDL_SCANCODE_S] || currentKeyStates[SDL_SCANCODE_D];

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
                    stopMovement();
                    sprite.clips = { player_idle_clips[lastDirection] };
                }
            }
        }
    }

    bool isMoving() const{
        return dx != 0 || dy !=0;
    }

    void updateHealth(Uint32 currentTime) {
        int elapsedSeconds = (currentTime - lastHealthDecrease) / 1000;
        int lossRate = 1 + (elapsedSeconds / 10);

        if (currentTime - lastHealthDecrease > healDecreaseRate) {
            health = std::max(0, health - 1);
            lastHealthDecrease = currentTime;
        }
    }

    void renderHealthBar(SDL_Renderer* renderer) {
        int barWidth = 200;
        int barHeight = 20;
        int x = (SCREEN_WIDTH - barWidth) / 2;
        int y = 10;

        SDL_Rect background = { x, y, barWidth, barHeight};
        SDL_Rect healthBar = { x, y, (health * barWidth) / 100, barHeight};

        SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
        SDL_RenderFillRect(renderer, &background);

        if (health > 50) {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        }
        else if (health > 20) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        }
        else {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        }
        SDL_RenderFillRect(renderer, &healthBar);
        }
};

struct CTX {
    SDL_Window* window;
    SDL_Renderer* renderer;
    Player player;
    std::vector<Enemy> enemies;
    SDL_Texture* backgroundTexture = nullptr;
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
    ctx.backgroundTexture = IMG_LoadTexture(ctx.renderer, BACKGROUND_FILE);

    SDL_Texture* texture = IMG_LoadTexture(ctx.renderer, PLAYER_SPRITE_FILE);
    ctx.player.sprite.texture = texture;
    ctx.player.sprite.init(texture, PLAYER_FRAMES, player_idle_clips);

    SDL_Texture* enemyTexture = IMG_LoadTexture(ctx.renderer, ENEMY_SPRITE_FILE);
        for (auto& enemy : ctx.enemies) {
            enemy.sprite.texture = enemyTexture;
            enemy.sprite.init(enemyTexture, PLAYER_FRAMES, enemy_movingsouth_clips);
        }
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

    ctx.player.updateHealth(SDL_GetTicks());
}
inline void renderBackground(CTX& ctx) {
    SDL_Rect bgRect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_RenderCopy(ctx.renderer, ctx.backgroundTexture, NULL, &bgRect);
}

inline void renderPlayer(CTX& ctx) {
    Sprite& sprite = ctx.player.sprite;
    const SDL_Rect* currentClip = &sprite.clips[sprite.currentFrame];

    SDL_Rect destRect = {
        ctx.player.x - (currentClip->w - PLAYER_WIDTH) / 2,
        ctx.player.y,
        currentClip->w, currentClip->h
    };

    SDL_RenderCopy(ctx.renderer, sprite.texture, currentClip, &destRect);
}

inline void spawnEnemy(CTX& ctx) {
    int x = rand() % SCREEN_WIDTH;
    int y = (SCREEN_HEIGHT / 2) + (rand() % (SCREEN_HEIGHT / 2 - ENEMY_HEIGHT));

    SDL_Texture* enemyTexture = IMG_LoadTexture(ctx.renderer, ENEMY_SPRITE_FILE);
    Enemy newEnemy(x, y, enemyTexture);

    newEnemy.sprite.init(enemyTexture, ENEMY_FRAMES, enemy_movingsouth_clips);

    ctx.enemies.push_back(newEnemy);
}

inline void renderEnemies(CTX& ctx) {
    for (auto& enemy : ctx.enemies) {
        enemy.render(ctx.renderer);
    }
}

inline void updateGame(CTX& ctx, Uint32 currentTime) {
    Player& player = ctx.player;

    player.updateAnimation(currentTime);
    if (player.isAttacking) {
        player.move();
        player.speed = INITIAL_SPEED / 2;
    }
    else {
        player.speed = INITIAL_SPEED;
        if (player.isMoving()) {
            int newDirection = (player.dy > 0) ? 0 : (player.dy < 0) ? 1 : (player.dx < 0) ? 2 : 3;
            if (player.direction != newDirection) {
                player.setDirection(newDirection, player.sprite.texture);
            }
            player.sprite.tick(currentTime);
            player.move();
            if (player.isMoving()) {
                player.lastDirection = player.direction;
            }

        }
        else {
            player.stopMovement();
            player.sprite.clips = { player_idle_clips[player.lastDirection] };
        }
    }
    player.updateHealth(currentTime);

    for (auto& enemy : ctx.enemies) {
        enemy.update(player.rect);
    }

    constexpr int MAX_ENEMIES = 3;
    if (ctx.enemies.size() < MAX_ENEMIES) {
        spawnEnemy(ctx);
    }
}

inline void render(CTX& ctx) {
    renderBackground(ctx);
    renderPlayer(ctx);
    renderEnemies(ctx);
    ctx.player.renderHealthBar(ctx.renderer);

    SDL_RenderPresent(ctx.renderer);
}

inline void cleanup(CTX& ctx) {
    SDL_DestroyTexture(ctx.backgroundTexture);
}

inline bool gameOver(const Player& player) {
    return player.health <=0;
}
#endif // GAME_H
