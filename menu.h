#ifndef MENU_H
#define MENU_H

#include <SDL.h>
#include "button.h"

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

#endif // MENU_H

