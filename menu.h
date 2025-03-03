#ifndef MENU_H
#define MENU_H

#include <SDL.h>
#include "buttonmanager.h"

bool showMenu(SDL_Renderer* renderer, ButtonManager& buttonManager) {
    bool running = true;
    bool startGame = false;
    bool showHelp = false;
    SDL_Event event;

    while (running) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (showHelp) {
            SDL_Texture* helpTexture = IMG_LoadTexture(renderer, "allpics/goose.jpg");
            SDL_Rect helpRect = {100, 100, 600, 400};
            SDL_RenderCopy(renderer, helpTexture, NULL, &helpRect);
            buttonManager.xbutton.render(renderer);
            SDL_DestroyTexture(helpTexture);
        }
        else {
            buttonManager.playbutton.render(renderer);
            buttonManager.quitbutton.render(renderer);
            buttonManager.helpbutton.render(renderer);
        }

        SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);

                if (buttonManager.playbutton.isClicked(x, y)) {
                    startGame = true;
                    running = false;
                }
                if (buttonManager.quitbutton.isClicked(x, y)) {
                    startGame = false;
                    running = false;
                }
                if (buttonManager.helpbutton.isClicked(x, y)) {
                    showHelp = true;
                }
                if (buttonManager.xbutton.isClicked(x, y)){
                    showHelp  = false;
                }
            }
        }
        SDL_Delay(16);
    }
    return startGame;
}

bool showPauseMenu(SDL_Renderer* renderer, ButtonManager& buttonManager) {
    bool pauseRunning = true;
    SDL_Event event;

    SDL_Texture* pauseTexture = IMG_LoadTexture(renderer, "allpics/goose.jpg");
    while (pauseRunning) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Màu nền mờ :0,0,0,200
        SDL_RenderClear(renderer);

        SDL_Rect pauseRect = {100, 100, 600, 400};
        SDL_RenderCopy(renderer, pauseTexture, NULL, &pauseRect);

        buttonManager.xbutton.render(renderer);

        SDL_RenderPresent(renderer);

        // Hiển thị các nút trong menu tạm dừng
        //buttonManager.playbutton.render(renderer);
        //buttonManager.restartbutton.render(renderer);
        //buttonManager.quitbutton.render(renderer);
        //SDL_RenderPresent(renderer);

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                pauseRunning = false;
                break;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);

                //if (buttonManager.playbutton.isClicked(x, y)) {
                //    return 1; // Tiếp tục game
                //}
                //if (buttonManager.restartbutton.isClicked(x, y)) {
                //    return -1; // Chơi lại
                //}
                //if (buttonManager.quitbutton.isClicked(x, y)) {
                //    return 0; // Thoát về menu chính
                //}
                if (buttonManager.xbutton.isClicked(x, y)) {
                    pauseRunning = false;
                    break;
                }
            }
        }
        SDL_Delay(16);
    }

    SDL_DestroyTexture(pauseTexture);
    return true;
}

struct Overlay {
    SDL_Renderer* renderer;
    SDL_Texture* overlayTexture;
    SDL_Rect pauseRect;

    Overlay(SDL_Renderer* renderer) : renderer(renderer), overlayTexture(nullptr) {
        pauseRect = {20, 20, 50, 50};
        overlayTexture = IMG_LoadTexture(renderer, "allpics/smallbuttons/pausebutton.png");
    }

    ~Overlay() {
        SDL_DestroyTexture(overlayTexture);
    }

    void render() {
        SDL_RenderCopy(renderer, overlayTexture, NULL, &pauseRect);
    }

    bool isPauseClicked(int x, int y) {
        return (x >= pauseRect.x && x <= pauseRect.x + pauseRect.w &&
                y >= pauseRect.y && y <= pauseRect.y + pauseRect.h);
    }
};

#endif // MENU_H

