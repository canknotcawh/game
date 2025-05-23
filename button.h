#pragma once
#include "ui.h"
#include "buttondefs.h"
#include <string>

class Button : public UI
{
public:
    Button(SDL_Renderer* renderer, SDL_Texture* texture);
    ~Button();
    void render() override;

    bool IsButtonClick(SDL_Event* event);

public:
    void setTexture(const char* filename);
    void setText(const char* Text);
    void setFont(const char* filePath);
    void setFontSize(const int& size);
    void setRect(SDL_Rect& rect);
    void setPosition(int x, int y);

private:
    void initButton();
    void createTextTexture(const char* p_FontPath, const char* p_Text);
private:
    std::string m_ButtonText;
    std::string m_FontPath;
    int m_FontSize;
    SDL_Texture* m_ButtonTextTexture;
    SDL_Color m_ButtonTextColor;
};
