#ifndef _GAMEOBJECTS__H
#define _GAMEOBJECTS__H

#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include "game.h"
#include "graphics.h"

struct Obstacle {
    int x, y;
    int width, height;
    SDL_Texture* texture;
    SDL_Color color;

    void render(Graphics& graphics) {
        //SDL_Rect destRect = {x, y, width, height};
        //SDL_RenderCopy(graphics.renderer, texture, NULL, &destRect);
        if (texture == nullptr) {
            SDL_SetRenderDrawColor(graphics.renderer, color.r, color.g, color.b, color.a);
            SDL_Rect destRect = {x, y, width, height};
            SDL_RenderFillRect(graphics.renderer, &destRect);
        } else {
            SDL_Rect destRect = {x, y, width, height};
            SDL_RenderCopy(graphics.renderer, texture, NULL, &destRect);
        }
    }
};

std::vector<Obstacle> obstacles;

void initObstacles(Graphics& graphics) {

    Obstacle obs1 = {200, 300, 50, 50, nullptr, {255, 0, 0, 255}};  // Đỏ
    Obstacle obs2 = {400, 200, 60, 60, nullptr, {0, 255, 0, 255}};   // Xanh lá
    Obstacle obs3 = {600, 100, 70, 70, nullptr, {0, 0, 255, 255}};   // Xanh dương

    obstacles.push_back(obs1);
    obstacles.push_back(obs2);
    obstacles.push_back(obs3);
}

void renderObstacles(Graphics& graphics) {
    for (Obstacle& obstacle : obstacles) {
        obstacle.render(graphics);
    }

bool checkCollision(const Player& player, const Obstacle& obstacle) {
    SDL_Rect playerRect = {player.x, player.y, 64, 64};
    SDL_Rect obstacleRect = {obstacle.x, obstacle.y, obstacle.width, obstacle.height};

    return SDL_HasIntersection(&playerRect, &obstacleRect);
}

void checkCollisions(Player& player, const std::vector<Obstacle>& obstacles) {
    for (const Obstacle& obstacle : obstacles) {
        if (checkCollision(player, obstacle)) {
            player.stopMovement();
        }
    }
}
#endif // _GAMEOBJECTS__H




