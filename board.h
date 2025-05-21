#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "pieces.h"
#include "move.h"
#include "piecesmanager.h"
#include <array>

const int g_WINDOW_WIDTH  = 600;
const int g_WINDOW_HEIGHT = 600;

class Board {
public:
    Board(int p_SizeBoard,SDL_Renderer* p_Renderer);
    void drawBoard();
    void UpdatePlayer(Player p_Player);
    int getPieceSize() const ;
private:
    void resetBoardColor();
    void SetHighlight(unsigned const int row, unsigned const int col);
    void showHints(Player p_Player);
    void showPieceHightlight(unsigned const int row, unsigned const int col);

    void BlackPawnHint();
    void WhitePawnHint();
    void BlackKingHint();
    void WhiteKingHint();
    void QueenHint();
    void KnightHint();
    void RookHint();
    void BishopHint();
private:
    std::array<int, MAX_PIECES_SPACE  > m_Pieces;
    std::array<std::array<SDL_Color, 8>, 8> m_BoardColor;
    Player m_Player;
    Move moveManager;
    std::vector<MoveInfo> m_MoveHistory;
    int m_BoardSize;
    SDL_Renderer* m_Renderer;
    int m_PiecesSize = 75;
    int whiteKingX, whiteKingY;
    int blackKingX, blackKingY;
};
