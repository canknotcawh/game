#include "menu.h"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

void Menu::initButtons() {
    m_PlayButton = new Button(m_Renderer, LoadTexture("buttons/playbutton.png"));
    m_CustomButton = new Button(m_Renderer, LoadTexture("buttons/custombutton.png"));
    m_QuitButton = new Button(m_Renderer, LoadTexture("buttons/exitbutton.png"));
}

void Menu::setButtonProperty() {
    m_PlayButton->setPosition(107, 269);
    m_CustomButton->setPosition(107, 326);
    m_QuitButton->setPosition(107, 463);
}
Menu::Menu(SDL_Renderer* p_Renderer) : m_Renderer(p_Renderer), menuState(Menu_State::NONE) {
    m_BackgroundTexture = LoadTexture("chess.png");
    initButtons();
    setButtonProperty();
    soundManager.init();
}

SDL_Texture* Menu::LoadTexture(const std::string& filePath) {
    SDL_Texture* texture = IMG_LoadTexture(m_Renderer, filePath.c_str());
    if (!texture) {
        std::cerr << "Error loading texture: " << IMG_GetError() << std::endl;
        return nullptr;
    }
    return texture;
}
void Menu::DisplayMenu()
{
    SDL_RenderClear(m_Renderer);
    if (m_BackgroundTexture) {
        SDL_RenderCopy(m_Renderer, m_BackgroundTexture, NULL, NULL);
    }

	m_PlayButton->render();
	m_QuitButton->render();

	SDL_RenderPresent(m_Renderer);
}
int Menu::getMouseSelection() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            int mouseX = event.button.x;
            int mouseY = event.button.y;

            if (mouseY >= 250 && mouseY <= 350) {
                if (mouseX >= 150 && mouseX < 250) return 0;
                if (mouseX >= 250 && mouseX < 350) return 1;
                if (mouseX >= 350 && mouseX < 450) return 2;
                if (mouseX >= 450 && mouseX < 550) return 3;
            }
        }
    }
    return -1;
}
int Menu::showPromotionMenu(bool isWhite) {
    SDL_Texture* promotionTextures[4];

    promotionTextures[0] = LoadTexture(isWhite ? "Pieces/QueenW.png" : "Pieces/QueenB.png");
    promotionTextures[1] = LoadTexture(isWhite ? "Pieces/RookW.png" : "Pieces/RookB.png");
    promotionTextures[2] = LoadTexture(isWhite ? "Pieces/BishopW.png" : "Pieces/BishopB.png");
    promotionTextures[3] = LoadTexture(isWhite ? "Pieces/KnightW.png" : "Pieces/KnightB.png");

    int choice = -1;
    while (choice == -1) {
        SDL_RenderClear(m_Renderer);

        int startX = 600 / 2 - 200;
        for (int i = 0; i < 4; i++) {
            SDL_Rect rect = {startX + i * 110, 250, 100, 100};
            SDL_RenderCopy(m_Renderer, promotionTextures[i], nullptr, &rect);
        }

        SDL_RenderPresent(m_Renderer);
        choice = getMouseSelection();
    }


    for (int i = 0; i < 4; i++) {
        SDL_DestroyTexture(promotionTextures[i]);
    }

    return choice;
}
void Menu::DisplayGameOver(std::string result) {
    SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_Renderer);

    std::string displayText;
    if (result == "White") {
        displayText = "Checkmate! White Wins!";
    }
    else if (result == "Black") {
        displayText = "Checkmate! Black Wins!";
    }
    else if (result == "Draw") {
        displayText = "Stalemate! It's a Draw!";
    }
    else {
        displayText = "Game Over!";
    }

    TTF_Font* font = TTF_OpenFont("Cinzel.ttf", 32);
    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, displayText.c_str(), textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(m_Renderer, textSurface);
    SDL_Rect textRect = {600 / 2 - textSurface->w / 2, 600 / 2 - textSurface->h / 2, textSurface->w, textSurface->h};
    SDL_FreeSurface(textSurface);
    TTF_CloseFont(font);

    SDL_RenderCopy(m_Renderer, textTexture, nullptr, &textRect);
}

const Menu_State Menu::getMenuState() const {
    SDL_Event event;
    if (SDL_WaitEvent(&event)) {
        if (m_PlayButton->IsButtonClick(&event)) return Menu_State::PLAY;
        if (m_CustomButton->IsButtonClick(&event)) return Menu_State::CUSTOM;
        if (m_QuitButton->IsButtonClick(&event)) return Menu_State::QUIT;
        if (event.type == SDL_QUIT) return Menu_State::QUIT;
    }
    return Menu_State::NONE;
}
void Menu::DisplayCustomMenu() {
    DisplayMenu();

    SDL_Rect customWindow = {100, 100, 400, 400};
    SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(m_Renderer, &customWindow);
    SDL_SetRenderDrawColor(m_Renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(m_Renderer, &customWindow);

    TTF_Font* font = TTF_OpenFont("Cinzel.ttf", 30);
    SDL_Color textColor = {255, 255, 255, 255};

    SDL_Surface* volumeSurface = TTF_RenderText_Solid(font, "Volume", textColor);
    SDL_Texture* volumeTexture = SDL_CreateTextureFromSurface(m_Renderer, volumeSurface);
    SDL_Rect volumeRect = {300 - volumeSurface->w / 2, 190, volumeSurface->w, volumeSurface->h};
    SDL_RenderCopy(m_Renderer, volumeTexture, nullptr, &volumeRect);
    SDL_FreeSurface(volumeSurface);
    SDL_DestroyTexture(volumeTexture);

    SDL_Rect sliderBar = {200, 240, 200, 10};
    SDL_SetRenderDrawColor(m_Renderer, 150, 150, 150, 255);
    SDL_RenderFillRect(m_Renderer, &sliderBar);

    SDL_Rect sliderKnob = {volumeSliderX - 5, 235, 10, 20};
    SDL_SetRenderDrawColor(m_Renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(m_Renderer, &sliderKnob);

    SDL_Surface* titleSurface = TTF_RenderText_Solid(font, "Time", textColor);
    SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(m_Renderer, titleSurface);
    SDL_Rect titleRect = {300 - titleSurface->w / 2, 280, titleSurface->w, titleSurface->h};
    SDL_RenderCopy(m_Renderer, titleTexture, nullptr, &titleRect);
    SDL_FreeSurface(titleSurface);
    SDL_DestroyTexture(titleTexture);

    std::string timeText;
    if (currentTimeOption == TimeOption::STANDARD) {
        timeText = "Standard";
    } else if (currentTimeOption == TimeOption::TEN_MINUTES) {
        timeText = "10 mins";
    } else {
        timeText = "30 mins";
    }

    SDL_Surface* timeSurface = TTF_RenderText_Solid(font, timeText.c_str(), textColor);
    SDL_Texture* timeTexture = SDL_CreateTextureFromSurface(m_Renderer, timeSurface);
    SDL_Rect timeRect = {300 - timeSurface->w / 2, 320, timeSurface->w, timeSurface->h};
    SDL_RenderCopy(m_Renderer, timeTexture, nullptr, &timeRect);
    SDL_FreeSurface(timeSurface);
    SDL_DestroyTexture(timeTexture);

    TTF_CloseFont(font);
    SDL_RenderPresent(m_Renderer);
}
void Menu::HandleCustomMenuEvent() {
    bool inCustomMenu = true;
    bool isDragging = false;

    while (inCustomMenu) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            int mouseX = event.button.x;
            int mouseY = event.button.y;

            if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (mouseX < 100 || mouseX > 500 || mouseY < 100 || mouseY > 500) {
                    menuState = Menu_State::NONE;
                    inCustomMenu = false;
                }
                if (mouseY >= 235 && mouseY <= 255) {
                    volumeSliderX = clamp(mouseX, 200, 400);
                    currentVolume = ((volumeSliderX - 200) * 128) / 200;
                    soundManager.setVolume(currentVolume);
                    isDragging = true;
                }

                if (mouseX >= 250 && mouseX <= 350 && mouseY >= 300 && mouseY <= 350) {
                    if (currentTimeOption == TimeOption::STANDARD) {
                        currentTimeOption = TimeOption::TEN_MINUTES;
                    } else if (currentTimeOption == TimeOption::TEN_MINUTES) {
                        currentTimeOption = TimeOption::THIRTY_MINUTES;
                    } else {
                        currentTimeOption = TimeOption::STANDARD;
                    }
                }
            }

            if (event.type == SDL_MOUSEBUTTONUP) {
                isDragging = false;
            }

            if (event.type == SDL_MOUSEMOTION && isDragging) {
                volumeSliderX = clamp(mouseX, 200, 400);
                currentVolume = ((volumeSliderX - 200) * 128) / 200;
                soundManager.setVolume(currentVolume);
            }
        }
        DisplayCustomMenu();
    }
}
Menu::~Menu() {
    if (m_BackgroundTexture) {
        SDL_DestroyTexture(m_BackgroundTexture);
    }
}
