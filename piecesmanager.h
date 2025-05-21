#pragma once
#include <string>
#include <SDL.h>
#include "pieces.h"
#include "move.h"
#include "board.h"
#include <array>
#include "menu.h"

class PiecesManager {
public:
    PiecesManager(SDL_Renderer* p_Renderer, int BoardSize);
    ~PiecesManager();

    std::array<std::array<int, 8>, 8> getBoard() const;
    std::vector<MoveInfo> moveHistory;
    void drawPieces();
    bool isValidMove(PiecePosition from, PiecePosition to);
    bool movePiece(int fromRow, int fromCol, int toRow, int toCol);
    void promotePawn(int row, int col, bool isWhite);
    void resetPieces();
    void updateBoardPieces(int* p_MouseX, int* p_MouseY);
    void isPieceSelect(bool p_state, int* p_MouseX, int* p_MouseY);

    void setSize(int p_PieceSize);
    std::string GameOver();

    int turn;
    Player getPlayer() const;
    Move moveManager;

private:
    void init();
    void initDefaultBoard();
    bool isBlack();
    void addPiece(const char* p_FilePath, int row, int col);
    void CalculatePieces();

private:
    SDL_Renderer* m_Renderer;
    int m_BoardPieceSize;
    bool m_PieceSelectState;
    int m_LastPiece_Row;
    int m_LastPiece_Col;
    bool m_IsLastBlackMove;
    Piece** m_DrawPieces;
    std::array<std::array<int, 8>, 8> m_BoardPieces;
    Menu menu;

    int whiteKingX = -1, whiteKingY = -1;
    int blackKingX = -1, blackKingY = -1;
};
