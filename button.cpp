#include "button.h"
#include "texturemanager.h"
#include <iostream>

void Button::initButton()
{
	m_FontPath = "";
	m_FontSize = 19;

	UI_Box = {};
	m_ButtonTextColor = {0, 0, 255, 255};
	UI_Color = {0, 0, 0, 0};
	m_ButtonTextTexture = NULL;
}

Button::Button(SDL_Renderer* p_Renderer, SDL_Texture* texture) {
    UI_Renderer = p_Renderer;
    UI_Texture = texture;
    UI_Box = {0, 0, 382, 80};
}


Button::~Button()
{
	SDL_DestroyTexture(m_ButtonTextTexture);
}

void Button::render() {
    if (UI_Texture) {
        SDL_RenderCopy(UI_Renderer, UI_Texture, NULL, &UI_Box);
    }
    else {
        SDL_SetRenderDrawColor(UI_Renderer, UI_Color.r, UI_Color.g, UI_Color.b, UI_Color.a);
        SDL_RenderFillRect(UI_Renderer, &UI_Box);
        SDL_RenderCopy(UI_Renderer, m_ButtonTextTexture, NULL, &UI_Box);
    }
}

bool Button::IsButtonClick(SDL_Event* event)
{
		if (event->type == SDL_MOUSEBUTTONUP)
		{
			if (SDL_BUTTON_LEFT == event->button.button)
			{
				int x,  y;
				SDL_GetMouseState(&x, &y);
				if ((UI_Box.x < x && x < UI_Box.x + UI_Box.w) &&
					(UI_Box.y < y && y < UI_Box.y + UI_Box.h))
				{
					return true;
				}
			}
		}
	return false;
}

void Button::setTexture(const char* filename)
{
	UI_Texture = TextureManager::GetTexture(UI_Renderer, filename);
	if (UI_Texture == NULL)
	{
		SDL_Log("Failed in loading the texture to button  PATH ->%s",filename);
	}
}

void Button::setText(const char* Text)
{
	m_ButtonText = Text;
	createTextTexture(m_FontPath.c_str(), m_ButtonText.c_str());
}

void Button::setFont(const char* filePath)
{
	m_FontPath = filePath;
	createTextTexture(m_FontPath.c_str(), m_ButtonText.c_str());
}

void Button::setFontSize(const int& size)
{
	m_FontSize = size;
	createTextTexture(m_FontPath.c_str(), m_ButtonText.c_str());
}

void Button::setRect(SDL_Rect& rect)
{
	UI_Box = rect;
}

void Button::setPosition(int x, int y)
{
	UI_Box.x = x;
	UI_Box.y = y;
}

void Button::createTextTexture(const char* p_FontPath,const char* p_Text)
{
	TTF_Font* _font = TTF_OpenFont(p_FontPath, m_FontSize);

	SDL_Surface* _surface = TTF_RenderText_Solid(_font,p_Text,m_ButtonTextColor);

	m_ButtonTextTexture = SDL_CreateTextureFromSurface(UI_Renderer, _surface);
	if (m_ButtonTextTexture == NULL)
	{
		SDL_Log("Failed to create Text Texture!!" );
	}
	SDL_Log("Error::BUTTON::TEXT %s ", SDL_GetError());
	TTF_CloseFont(_font);
	SDL_FreeSurface(_surface);
}
