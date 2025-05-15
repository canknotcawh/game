#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "pieces.h"
#include <array>

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

    void WhitePawnHint();
    void BlackPawnHint();
    void KingHint();
    void QueenHint();
    void KnightHint();
    void RookHint();
    void BishopHint();
private:
    std::array<int, MAX_PIECES_SPACE  > m_Pieces;
    std::array<std::array<SDL_Color, MAX_PIECES_LINE>, MAX_PIECES_LINE> m_BoardColor;
    Player m_Player;
    int m_BoardSize;
    SDL_Renderer* m_Renderer;
    int m_PiecesSize = 75;
};




