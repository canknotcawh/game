#pragma once
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "board.h"
#include "piecesmanager.h"
#include "soundmanager.h"

class Game
{
public:
    Game(const char* p_Title, int p_Width,int p_Height);
    ~Game();
    void update();
    void pollEvent();
    bool isRunning()const;
    void resetGame();
    void render();
    std::string getWinner(){ return m_BoardPieces->GameOver(); }

    SDL_Renderer* getRenderer()const;
    SDL_Window* getWindow()const { return m_Window; }
private:
    void init();
    void updateMousePosition();
    void getMousePosition(int* x,int * y);
private:
    int *m_CurrentmouseX;
    int *m_CurrentmouseY;

    SDL_Event* m_Event;
    SDL_Window* m_Window;
    SDL_Renderer* m_Renderer;
    bool m_Running = false;
    bool m_Is_Selected;

    Board* m_Board;
    PiecesManager* m_BoardPieces;
    SoundManager soundManager;
    bool hasPlayedMoveSound = false;
};
