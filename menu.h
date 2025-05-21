#pragma once

#include <string>
#include "button.h"
#include "menudefs.h"
#include <vector>
#include "soundmanager.h"

class Menu
{
public:
    SDL_Texture* LoadTexture(const std::string& filePath);
    Menu(SDL_Renderer* p_Renderer);
    ~Menu();
    void DisplayMenu();
    int showPromotionMenu(bool isWhite);
    int getMouseSelection();
    void DisplayGameOver(std::string winner);

    const Menu_State getMenuState()const;
    void DisplayCustomMenu();
    void HandleCustomMenuEvent();

private:
    void initButtons();
    void setButtonProperty();
    SDL_Texture* m_BackgroundTexture;
    Button* m_PlayButton;
    Button* m_CustomButton;
    Button* m_QuitButton;
    SDL_Renderer* m_Renderer;
    SDL_Event m_Event;
    Menu_State menuState;

    SoundManager soundManager;
    int volumeSliderX = 300;
    int currentVolume = 80;

    enum class TimeOption { STANDARD, TEN_MINUTES, THIRTY_MINUTES };
    TimeOption currentTimeOption = TimeOption::STANDARD;
};
