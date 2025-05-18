#include "texturemanager.h"
#include <SDL_image.h>
#include <cstddef>

SDL_Texture* TextureManager::GetTexture(SDL_Renderer* p_Renderer,const char *p_FilePath)
{
    SDL_Texture* _texture = IMG_LoadTexture(p_Renderer,p_FilePath);
    if (_texture == NULL )
    {
        SDL_Log("Failed load the image from %s \n "
                " {ERROR: %s }\n",p_FilePath,SDL_GetError());
        return NULL;
    }
    return _texture;
}

void TextureManager::Render(SDL_Renderer *p_Renderer, SDL_Texture *p_Texture,
        SDL_Rect* p_DsntRect)
{
    SDL_Rect _srcRect = {0,0,60,60};
    SDL_RenderCopy(p_Renderer,p_Texture,NULL,p_DsntRect);
}

