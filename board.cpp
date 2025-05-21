#include "board.h"
#include "colordefs.h"
#include <iostream>

Board::Board(int p_SizeBoard, SDL_Renderer* p_Renderer)
    : m_BoardSize(p_SizeBoard), m_Renderer(p_Renderer)
{
    m_PiecesSize = m_BoardSize / 8;
}

void Board::resetBoardColor() {
    for (int row = 0; row <= 7; row++) {
        for (int column = 0; column <= 7; column++) {
            if ((row + column) % 2 == 0) {
                m_BoardColor[row][column] = SDL_Color{222, 184, 135, 255};
            }
            else {
                m_BoardColor[row][column] = SDL_Color{119, 73, 49, 255};
            }
        }
    }
}

void Board::UpdatePlayer(Player p_Player)
{
    m_Player = p_Player;

    resetBoardColor();

}

void Board::drawBoard()
{
    int boardPixelWidth = 8 * m_PiecesSize;
    int boardOffsetX = (g_WINDOW_WIDTH - boardPixelWidth) / 2;

    for (int row = 0; row <= 7; row++)
    {
        for (int column = 0; column <= 7; column++)
        {
            SDL_Rect _block = {};
            _block.x = boardOffsetX + column * m_PiecesSize;
            _block.y = row * m_PiecesSize;
            _block.w = _block.h = m_PiecesSize;

            SDL_SetRenderDrawColor(m_Renderer,
                m_BoardColor[row][column].r,
                m_BoardColor[row][column].g,
                m_BoardColor[row][column].b,
                m_BoardColor[row][column].a);

            SDL_RenderFillRect(m_Renderer, &_block);
        }
    }
}

int Board::getPieceSize() const
{
    return m_PiecesSize;
}

