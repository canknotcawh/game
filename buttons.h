#ifndef BUTTONS_H
#define BUTTONS_H

#include <SDL.h>
#include <SDL_image.h>

struct Button {
    SDL_Texture* texture;
    SDL_Rect rect;

    Button(SDL_Renderer* renderer, const char* filePath, int x, int y, int w, int h) {
        SDL_Surface* surface = IMG_Load(filePath);
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        rect = { x, y, w, h };
    }

    ~Button() {
        SDL_DestroyTexture(texture);
    }

    void render(SDL_Renderer* renderer) {
        SDL_RenderCopy(renderer, texture, NULL, &rect);
    }

    bool isClicked(int mouseX, int mouseY) {
        return (mouseX >= rect.x && mouseX <= rect.x + rect.w &&
                mouseY >= rect.y && mouseY <= rect.y + rect.h);
    }
};

struct ButtonManager {
    Button playbutton;
    Button pausebutton;
    Button quitbutton;
    Button xbutton;
    Button helpbutton;

    ButtonManager(SDL_Renderer* renderer)
        : playbutton(renderer, "allpics/largebuttons/playbutton.png", 340, 400, 120, 50),
          pausebutton(renderer, "allpics/smallbuttons/pausebutton.png", 0, 0, 50, 50),
          quitbutton(renderer, "allpics/largebuttons/quitbutton.png", 340, 460, 120, 50),
          xbutton(renderer, "allpics/smallbuttons/xbutton.png", 100, 100, 30, 30),
          helpbutton(renderer, "allpics/smallbuttons/helpbutton.png", 470, 480, 30, 30){}


    void render(SDL_Renderer* renderer) {
        playbutton.render(renderer);
        pausebutton.render(renderer);
        quitbutton.render(renderer);
        xbutton.render(renderer);
    }

};

#endif // BUTTONS_H

