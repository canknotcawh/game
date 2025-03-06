#ifndef BUTTON_H
#define BUTTON_H

#include <SDL.h>
#include <SDL_image.h>
#include <cstdio>

struct Button {
    SDL_Texture* texture;
    SDL_Rect rect;

    Button() : texture(nullptr), rect{0, 0, 0, 0} {}

    Button(SDL_Renderer* renderer, const char* filePath, int x, int y, int w, int h) {
        SDL_Surface* surface = IMG_Load(filePath);
        if (!surface) {
            printf("Failed to load image %s: %s\n", filePath, IMG_GetError());
            texture = nullptr;
            return;
        }
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        rect = { x, y, w, h };
    }

    ~Button() {
        if (texture) SDL_DestroyTexture(texture);
    }

    void render(SDL_Renderer* renderer) {
        if (texture) SDL_RenderCopy(renderer, texture, NULL, &rect);
    }

    bool isClicked(int mouseX, int mouseY) {
        return (mouseX >= rect.x && mouseX <= rect.x + rect.w &&
                mouseY >= rect.y && mouseY <= rect.y + rect.h);
    }
};

struct ButtonManager {
    Button playbutton;
    Button quitbutton;
    Button xbutton;
    Button helpbutton;

    ButtonManager(SDL_Renderer* renderer)
        : playbutton(renderer, "allpics/largebuttons/playbutton.png", 340, 400, 120, 50),
          quitbutton(renderer, "allpics/largebuttons/quitbutton.png", 340, 460, 120, 50),
          xbutton(renderer, "allpics/smallbuttons/xbutton.png", 100, 100, 30, 30),
          helpbutton(renderer, "allpics/smallbuttons/helpbutton.png", 470, 480, 30, 30) {}

    void render(SDL_Renderer* renderer) {
        playbutton.render(renderer);
        quitbutton.render(renderer);
        xbutton.render(renderer);
        helpbutton.render(renderer);
    }
};

#endif // BUTTON_H
