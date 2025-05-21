#pragma once
#include <array>
#include <vector>
#include "pieces.h"

class MoveInfo
{
public:
    int startX, startY;
    int endX, endY;
    int pieceMoved;
    int pieceCaptured;

    MoveInfo() : startX(0), startY(0), endX(0), endY(0), pieceMoved(0), pieceCaptured(0) {}

    MoveInfo(int sx, int sy, int ex, int ey, int pm, int pc)
        : startX(sx), startY(sy), endX(ex), endY(ey), pieceMoved(pm), pieceCaptured(pc) {}
};

class Move
{
public:
    static bool isBlackKingInCheck(const std::array<std::array<int, 8>, 8>& board, const std::vector<MoveInfo>& moveHistory, int kingX, int kingY);
    static bool isWhiteKingInCheck(const std::array<std::array<int, 8>, 8>& board, const std::vector<MoveInfo>& moveHistory, int kingX, int kingY);
    static bool isPinned(const std::array<std::array<int, 8>, 8>& board, const std::vector<MoveInfo>& moveHistory, int pieceX, int pieceY);
    static std::vector<PiecePosition> LegalMoves(const std::array<std::array<int, 8>, 8>& board, const std::vector<MoveInfo>& moveHistory, int kingX, int kingY, int pieceX, int pieceY, bool isWhite);
    static bool canBlockCheck(const std::vector<MoveInfo>& moveHistory, const std::array<std::array<int, 8>, 8>& board, int kingX, int kingY, int pieceX, int pieceY, bool isWhite);
    static bool isCheckmate(const std::array<std::array<int, 8>, 8>& board, const std::vector<MoveInfo>& moveHistory, int kingX, int kingY, bool isWhite);
    static bool isStalemate(const std::array<std::array<int, 8>, 8>& board, const std::vector<MoveInfo>& moveHistory, int kingX, int kingY, bool isWhite);
    static bool isInsufficientMaterial(const std::array<std::array<int, 8>, 8>& board);
    static bool canCastle(const std::array<std::array<int, 8>, 8>& board, bool isWhite, const std::vector<MoveInfo>& moveHistory);
    static bool canEnPassantLeft(const std::array<std::array<int, 8>, 8>& board, bool isWhite, const std::vector<MoveInfo>& moveHistory, int pawnRow, int pawnCol);
    static bool canEnPassantRight(const std::array<std::array<int, 8>, 8>& board, bool isWhite, const std::vector<MoveInfo>& moveHistory, int pawnRow, int pawnCol);
    static bool ValidMove(const std::array<std::array<int, 8>, 8>& board, PiecePosition from, PiecePosition to, const std::vector<MoveInfo>& moveHistory);

    void addMove(int startX, int startY, int endX, int endY, int pieceMoved, int pieceCaptured);
    const std::vector<MoveInfo>& getMoveHistory() const;

private:
    std::vector<MoveInfo> moveHistory;
};

