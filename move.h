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
    static bool isBlackKingInCheck(const std::vector<MoveInfo>& moveHistory, const std::array<std::array<int, 8>, 8>& board, int kingX, int kingY);
    static bool isWhiteKingInCheck(const std::vector<MoveInfo>& moveHistory, const std::array<std::array<int, 8>, 8>& board, int kingX, int kingY);
    static bool ValidMove(std::array<std::array<int, MAX_PIECES_LINE>, MAX_PIECES_LINE> p_PieceBoard, PiecePosition p_OldPosition, PiecePosition p_NewPosition, const std::vector<MoveInfo>& moveHistory);

    void addMove(int startX, int startY, int endX, int endY, int pieceMoved, int pieceCaptured);
    const std::vector<MoveInfo>& getMoveHistory() const;

private:
    std::vector<MoveInfo> moveHistory;
};

