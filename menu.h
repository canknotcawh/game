#ifndef MENU_H
#define MENU_H

#include <SDL.h>
#include "button.h"

bool showMenu(SDL_Renderer* renderer, ButtonManager& buttonManager) {
    bool running = true;
    bool startGame = false;
    bool showHelp = false;
    SDL_Event event;

    SDL_Texture* menuBackground = IMG_LoadTexture(renderer, BACKGROUND_FILE);
    SDL_Texture* helpTexture = IMG_LoadTexture(renderer, "allpics/backgrounds/goose.jpg");

    while (running) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_RenderCopy(renderer, menuBackground, NULL, NULL);

        if (showHelp) {
            SDL_Rect helpRect = {100, 50, 600, 400};
            SDL_RenderCopy(renderer, helpTexture, NULL, &helpRect);
            buttonManager.xbutton.render(renderer);
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
            else if (event.type == SDL_MOUSEBUTTONDOWN) {
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
            else if (event.type == SDL_MOUSEMOTION) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                buttonManager.playbutton.checkHover(x, y);
                buttonManager.quitbutton.checkHover(x, y);
                buttonManager.helpbutton.checkHover(x, y);
                buttonManager.xbutton.checkHover(x, y);
            }
        }
        SDL_Delay(16);
    }

    SDL_DestroyTexture(menuBackground);
    SDL_DestroyTexture(helpTexture);
    return startGame;
}

#endif // MENU_H

