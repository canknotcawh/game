#include "game.h"
#include "menu.h"

static SoundManager soundManager;
static Game* gGame = nullptr;
SDL_Renderer* renderer = nullptr;

inline void init();
inline void GameLoop();
inline void close();

int main(int argc, char* argv[]) {
    init();
    GameLoop();
    close();
    return 0;
}

inline void init() {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
    TTF_Init();

    soundManager.init();
    soundManager.setVolume(80);
    soundManager.loadMusic("sounds/chessmusic.mp3");

    gGame = new Game("Chess", g_WINDOW_WIDTH, g_WINDOW_HEIGHT);
}

inline void GameLoop() {
    soundManager.playMusic(0, -1);

    bool MenuRunning = true;
    Menu menu(gGame->getRenderer());

    while (MenuRunning) {
        switch (menu.getMenuState()) {
            case Menu_State::NONE:
                menu.DisplayMenu();
                break;

            case Menu_State::PLAY: {
                soundManager.playSound(0);
                gGame->resetGame();

                while (gGame->isRunning()) {
                    gGame->pollEvent();
                    gGame->update();
                    gGame->render();

                    std::string winner = gGame->getWinner();
                    if (!winner.empty()) {
                        menu.DisplayGameOver(winner);
                        SDL_RenderPresent(gGame->getRenderer());
                        SDL_Delay(2000);

                        menu.DisplayMenu();
                        break;
                    }
                }
                break;
            }
            case Menu_State::CUSTOM:
                menu.DisplayCustomMenu();
                menu.HandleCustomMenuEvent();
                break;

            case Menu_State::QUIT:
                MenuRunning = false;
                break;
        }
    }
}

inline void close() {
    soundManager.stopMusic();
    soundManager.clean();

    delete gGame;

    SDL_Quit();
    IMG_Quit();
}
