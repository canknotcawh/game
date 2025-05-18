#pragma once
#include <array>

#define BLACK_PAWN_PATH "Pieces/PawnB.png"
#define BLACK_BISHOP_PATH "Pieces/BishopB.png"
#define BLACK_KNIGHT_PATH "Pieces/KnightB.png"
#define BLACK_ROOK_PATH "Pieces/RookB.png"
#define BLACK_QUEEN_PATH "Pieces/QueenB.png"
#define BLACK_KING_PATH "Pieces/KingB.png"

#define WHITE_PAWN_PATH "Pieces/PawnW.png"
#define WHITE_BISHOP_PATH "Pieces/BishopW.png"
#define WHITE_KNIGHT_PATH "Pieces/KnightW.png"
#define WHITE_ROOK_PATH "Pieces/RookW.png"
#define WHITE_QUEEN_PATH "Pieces/QueenW.png"
#define WHITE_KING_PATH "Pieces/KingW.png"
#define EMPTY_PATH "Pieces/EMPTY.png"

#define MAX_PIECES_SPACE 64
#define MAX_DIFFERENT_TYPES_PIECES 16

namespace PIECES_TYPE {
    const int EMPTY = 0;
    const int WHITE_KING = 1;
    const int WHITE_QUEEN = 2;
    const int WHITE_BISHOP = 3;
    const int WHITE_KNIGHT = 4;
    const int WHITE_ROOK = 5;
    const int WHITE_PAWN = 6;
    const int BLACK_KING = -1;
    const int BLACK_QUEEN = -2;
    const int BLACK_BISHOP = -3;
    const int BLACK_KNIGHT = -4;
    const int BLACK_ROOK = -5;
    const int BLACK_PAWN = -6;
};

struct Player
{
    Player(std::array<std::array<int, 8>, 8> p_BoardPiece)
        : BoardPieces(p_BoardPiece) ,row (0),col(0),isSeleted(false)
    {}
    Player() : row(0), col(0), isSeleted(false){}

    void setPosition(int row, int col) { this->row = row; this->col = col; }
    void setBoardPieces(std::array<std::array<int, 8>, 8> BoardPieces) { this->BoardPieces = BoardPieces; }
    void setIsSelected(bool state) { isSeleted = state; }

    std::array<std::array<int, 8>, 8> BoardPieces;
    int row;
    int col;
    bool isSeleted;
};
