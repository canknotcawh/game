#ifndef ENEMY_H
#define ENEMY_H

#define ENEMY_SPEED 1

#include <SDL.h>
#include <SDL_image.h>
#include <random>
#include <cstdlib>
#include "graphics.h"
#include "defs.h"

struct Vector2DInt {
    int x, y;
    Vector2DInt(int x = 0, int y = 0) : x(x), y(y) {}
};

struct Level {
    Vector2DInt getRandomEnemySpawnerLocation() {
        static std::random_device rd;
        static std::mt19937 gen(rd());

        std::uniform_int_distribution<int> sideDist(0, 1);
        std::uniform_int_distribution<int> distY(50, SCREEN_HEIGHT - PLAYER_HEIGHT - 50);

        int side = sideDist(gen);
        int x = (side == 0) ? -ENEMY_WIDTH : SCREEN_WIDTH;
        int y = distY(gen);

        return Vector2DInt(x, y);
    }
};

enum Direction { NONE, NORTH, SOUTH, EAST, WEST };

struct Enemy {
    SDL_Point position;
    int speed;
    SDL_Rect rect;
    Sprite sprite;
    int frameDelay;
    int frameCounter;
    Direction moveDirection;

    Enemy(int x, int y, SDL_Texture* texture) {
        position.x = x;
        position.y = y;
        speed = ENEMY_SPEED;

        rect = { x, y, ENEMY_WIDTH, ENEMY_HEIGHT };
        sprite.texture = texture;
        sprite.init(texture, ENEMY_FRAMES, enemy_movingsouth_clips);

        frameDelay = 6;
        frameCounter = 0;
        moveDirection = NONE;
    }

    void update(const SDL_Rect& playerRect) {
        int dx = playerRect.x - position.x;
        int dy = playerRect.y - position.y;
        if (std::abs(dx) <= speed && std::abs(dy) <= speed) {
            dx = 0;
            dy = 0;
        }
        else {
            if (moveDirection == NONE || (moveDirection == EAST && position.x >= playerRect.x) || (moveDirection == WEST && position.x <= playerRect.x) || (moveDirection == SOUTH && position.y >= playerRect.y) || (moveDirection == NORTH && position.y <= playerRect.y)) {

                if (std::abs(dx) > std::abs(dy)) {
                    moveDirection = (dx > 0) ? EAST : WEST;
                }
                else {
                    moveDirection = (dy > 0) ? SOUTH : NORTH;
                }
            }
        }

        switch (moveDirection) {
            case EAST:
                position.x += speed;
                sprite.clips = std::vector<SDL_Rect>(std::begin(enemy_movingeast_clips), std::end(enemy_movingeast_clips));
                break;
            case WEST:
                position.x -= speed;
                sprite.clips = std::vector<SDL_Rect>(std::begin(enemy_movingwest_clips), std::end(enemy_movingwest_clips));
                break;
            case SOUTH:
                position.y += speed;
                sprite.clips = std::vector<SDL_Rect>(std::begin(enemy_movingsouth_clips), std::end(enemy_movingsouth_clips));
                break;
            case NORTH:
                position.y -= speed;
                sprite.clips = std::vector<SDL_Rect>(std::begin(enemy_movingnorth_clips), std::end(enemy_movingnorth_clips));
                break;
            case NONE:
                break;
        }

        if (dx != 0 || dy != 0 || moveDirection != NONE) {
        switch (moveDirection) {
            case EAST:
                sprite.clips.assign(std::begin(enemy_movingeast_clips), std::end(enemy_movingeast_clips));
                break;
            case WEST:
                sprite.clips.assign(std::begin(enemy_movingwest_clips), std::end(enemy_movingwest_clips));
                break;
            case SOUTH:
                sprite.clips.assign(std::begin(enemy_movingsouth_clips), std::end(enemy_movingsouth_clips));
                break;
            case NORTH:
                sprite.clips.assign(std::begin(enemy_movingnorth_clips), std::end(enemy_movingnorth_clips));
                break;
            default:
                break;
        }
    }

        frameCounter++;
        if (frameCounter >= frameDelay) {
            sprite.currentFrame = (sprite.currentFrame + 1) % sprite.clips.size();
            frameCounter = 0;
        }

        rect.x = position.x;
        rect.y = position.y;
    }

    bool isCollidingWithOtherEnemies(const Vector2DInt& pos, const std::vector<Enemy>& enemies) {
        for (const auto& enemy : enemies) {
            int distX = std::abs(enemy.position.x - pos.x);
            int distY = std::abs(enemy.position.y - pos.y);
            if (distX < ENEMY_WIDTH && distY < ENEMY_HEIGHT) {
                return true;
            }
        }
        return false;
    }

    Vector2DInt getValidEnemySpawnLocation(const std::vector<Enemy>& enemies) {
        Vector2DInt pos;
        do {
            pos = Level().getRandomEnemySpawnerLocation();
        } while (isCollidingWithOtherEnemies(pos, enemies));
        return pos;
    }

    void render(SDL_Renderer* renderer) {
        SDL_Rect enemyRect = { position.x, position.y, ENEMY_WIDTH, ENEMY_HEIGHT };
        SDL_RenderCopy(renderer, sprite.texture, &sprite.clips[sprite.currentFrame], &enemyRect);
    }
};

#endif // ENEMY_H
