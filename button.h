#ifndef BUTTON_H
#define BUTTON_H

#include <SDL.h>
#include <SDL_image.h>
#include <cstdio>

struct Button {
    SDL_Texture* texture;
    SDL_Texture* hovertexture;
    SDL_Rect rect;
    bool isHovered;

    Button() : texture(nullptr), hovertexture(nullptr), rect{0, 0, 0, 0}, isHovered(false) {}

    Button(SDL_Renderer* renderer, const char* filePath, int x, int y, int w, int h) {
        SDL_Surface* surface = IMG_Load(filePath);
        if (!surface) {
            printf("Failed to load image %s: %s\n", filePath, IMG_GetError());
            texture = nullptr;
            return;
        }
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);

        surface = IMG_Load("allpics\backgrounds\goose.jpg");
        if (!surface) {
            printf("Failed to load hover image %s: %s\n", "allpics\backgrounds\goose.jpg", IMG_GetError());
            hovertexture = nullptr;
        }
        else {
            hovertexture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_FreeSurface(surface);
        }

        rect = { x, y, w, h };
        isHovered = false;
    }

    ~Button() {
        if (texture) SDL_DestroyTexture(texture);
        if (hovertexture) SDL_DestroyTexture(hovertexture);
    }

    void render(SDL_Renderer* renderer) {
        if (texture) {
            if (isHovered && hovertexture) {
                SDL_RenderCopy(renderer, hovertexture, NULL, &rect);
            }
            else {
                SDL_RenderCopy(renderer, texture, NULL, &rect);
            }
        }
    }

    bool isClicked(int mouseX, int mouseY) {
        return (mouseX >= rect.x && mouseX <= rect.x + rect.w &&
                mouseY >= rect.y && mouseY <= rect.y + rect.h);
    }

    void checkHover(int mouseX, int mouseY) {
        isHovered = (mouseX >= rect.x && mouseX <= rect.x + rect.w &&
                     mouseY >= rect.y && mouseY <= rect.y + rect.h);
    }
};

struct ButtonManager {
    Button playbutton;
    Button quitbutton;
    Button xbutton;
    Button helpbutton;

    ButtonManager(SDL_Renderer* renderer)
        : playbutton(renderer, "allpics/largebuttons/playbutton.png", 340, 300, 120, 50),
          quitbutton(renderer, "allpics/largebuttons/quitbutton.png", 340, 360, 120, 50),
          xbutton(renderer, "allpics/smallbuttons/xbutton.png", 100, 50, 30, 30),
          helpbutton(renderer, "allpics/smallbuttons/helpbutton.png", 470, 380, 30, 30) {}

    void render(SDL_Renderer* renderer) {
        playbutton.render(renderer);
        quitbutton.render(renderer);
        xbutton.render(renderer);
        helpbutton.render(renderer);
    }

    void checkHover(int mouseX, int mouseY) {
        playbutton.checkHover(mouseX, mouseY);
        quitbutton.checkHover(mouseX, mouseY);
        xbutton.checkHover(mouseX, mouseY);
        helpbutton.checkHover(mouseX, mouseY);
    }
};

#endif // BUTTON_H
