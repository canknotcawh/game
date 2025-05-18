#include "move.h"
#include "piecesdefs.h"
#include "pieces.h"
#include <iostream>
static bool isBlack(const std::array<std::array<int, 8>, 8>& board , PiecePosition to)
{
    return (0 > board[to.Row][to.Col]) ? true : false;
}
static bool isWhite(const std::array<std::array<int, 8>, 8>& board , PiecePosition to)
{
    return (0 < board[to.Row][to.Col]) ? true : false;
}

void Move::addMove(int startX, int startY, int endX, int endY, int pieceMoved, int pieceCaptured) {
    MoveInfo move = {startX, startY, endX, endY, pieceMoved, pieceCaptured};
    moveHistory.push_back(move);
}
const std::vector<MoveInfo>& Move::getMoveHistory() const {
    return moveHistory;
}

bool Move::isBlackKingInCheck(const std::vector<MoveInfo>& moveHistory, const std::array<std::array<int, 8>, 8>& board, int blackKingX, int blackKingY) {
    for (int row = 0; row <= 7; ++row) {
        for (int col = 0; col <= 7; ++col) {
            int piece = board[row][col];
            if (piece > 0) {
                if (Move::ValidMove(board, PiecePosition(row, col), PiecePosition(blackKingX, blackKingY), moveHistory)) {
                    return true;
                }
            }
        }
    }
    return false;
}
bool Move::isWhiteKingInCheck(const std::vector<MoveInfo>& moveHistory, const std::array<std::array<int, 8>, 8>& board, int whiteKingX, int whiteKingY) {
    for (int row = 0; row <= 7; ++row) {
        for (int col = 0; col <= 7; ++col) {
            int piece = board[row][col];
            if (piece < 0) {
                if (Move::ValidMove(board, PiecePosition(row, col), PiecePosition(whiteKingX, whiteKingY), moveHistory)) {
                    return true;
                }
            }
        }
    }
    return false;
}
bool Move::canCastle(const std::array<std::array<int, 8>, 8>& board, bool isWhite, const std::vector<MoveInfo>& moveHistory)
{
    int row = isWhite ? 7 : 0;
    PiecePosition kingPos{row, 4};
    PiecePosition rookKingside{row, 7};
    PiecePosition rookQueenside{row, 0};

    for (const auto& move : moveHistory)
    {
        if (move.startX == kingPos.Row && move.startY == kingPos.Col)
            return false;
    }

    bool canKingsideCastle = true;
    for (const auto& move : moveHistory)
    {
        if (move.startX == rookKingside.Row && move.startY == rookKingside.Col)
            canKingsideCastle = false;
    }

    if (board[row][5] != PIECES_TYPE::EMPTY || board[row][6] != PIECES_TYPE::EMPTY)
        canKingsideCastle = false;

    for (int col = 4; col <= 6; ++col)
    {
        if ((isWhite && Move::isWhiteKingInCheck(moveHistory, board, row, col)) ||
            (!isWhite && Move::isBlackKingInCheck(moveHistory, board, row, col)))
            canKingsideCastle = false;
    }

    bool canQueensideCastle = true;
    for (const auto& move : moveHistory)
    {
        if (move.startX == rookQueenside.Row && move.startY == rookQueenside.Col)
            canQueensideCastle = false;
    }

    if (board[row][1] != PIECES_TYPE::EMPTY || board[row][2] != PIECES_TYPE::EMPTY || board[row][3] != PIECES_TYPE::EMPTY)
        canQueensideCastle = false;

    for (int col = 4; col >= 1; --col)
    {
        if ((isWhite && Move::isWhiteKingInCheck(moveHistory, board, row, col)) ||
            (!isWhite && Move::isBlackKingInCheck(moveHistory, board, row, col)))
            canQueensideCastle = false;
    }

    return canKingsideCastle || canQueensideCastle;
}
bool possibleMoveBlackKing(const std::array<std::array<int, 8>, 8>& board, const PiecePosition& from, const PiecePosition& to, const std::vector<MoveInfo>& moveHistory)
{
    if (from.Row < 0 || from.Row > 7 || from.Col < 0 || from.Col > 7 ||
        to.Row < 0 || to.Row > 7 || to.Col < 0 || to.Col > 7)
        return false;

    int dRow = to.Row - from.Row;
    int dCol = to.Col - from.Col;
    if (abs(dRow) > 1 || abs(dCol) > 1)
        return false;

    int pieceAtStart = board[from.Row][from.Col];
    int pieceAtEnd = board[to.Row][to.Col];
    if ((pieceAtStart != PIECES_TYPE::BLACK_KING) && (pieceAtEnd < 0))
        return false;

    auto tempBoard = board;
    tempBoard[to.Row][to.Col] = pieceAtStart;
    tempBoard[from.Row][from.Col] = PIECES_TYPE::EMPTY;

    if (Move::isBlackKingInCheck(moveHistory, tempBoard, to.Row, to.Col))
        return false;

    return true;
}

bool possibleMoveWhiteKing(const std::array<std::array<int, 8>, 8>& board, const PiecePosition& from, const PiecePosition& to, const std::vector<MoveInfo>& moveHistory)
{
    if (from.Row < 0 || from.Row > 7 || from.Col < 0 || from.Col > 7 ||
        to.Row < 0 || to.Row > 7 || to.Col < 0 || to.Col > 7)
        return false;

    int dRow = to.Row - from.Row;
    int dCol = to.Col - from.Col;
    if (abs(dRow) > 1 || abs(dCol) > 1)
        return false;

    int pieceAtStart = board[from.Row][from.Col];
    int pieceAtEnd = board[to.Row][to.Col];
    if ((pieceAtStart != PIECES_TYPE::WHITE_KING) && (pieceAtEnd > 0))
        return false;

    auto tempBoard = board;
    tempBoard[to.Row][to.Col] = pieceAtStart;
    tempBoard[from.Row][from.Col] = PIECES_TYPE::EMPTY;

    if (Move::isWhiteKingInCheck(moveHistory, tempBoard, to.Row, to.Col))
        return false;

    return true;
}
bool Move::canEnPassant(const std::array<std::array<int, 8>, 8>& board, bool isWhite, const std::vector<MoveInfo>& moveHistory)
{
    if (moveHistory.empty())
        return false;

    const MoveInfo& lastMove = moveHistory.back();
    int enemyPawn = isWhite ? PIECES_TYPE::BLACK_PAWN : PIECES_TYPE::WHITE_PAWN;
    int row = isWhite ? 3 : 4;

    if (lastMove.pieceMoved == enemyPawn && abs(lastMove.startX - lastMove.endX) == 2)
    {
        if (lastMove.endX == row && (board[row][lastMove.endY + 1] == (isWhite ? PIECES_TYPE::WHITE_PAWN : PIECES_TYPE::BLACK_PAWN) ||
                                     board[row][lastMove.endY - 1] == (isWhite ? PIECES_TYPE::WHITE_PAWN : PIECES_TYPE::BLACK_PAWN)))
        {
            return true;
        }
    }
    return false;
}
bool possibleMoveBlackPawn(const std::array<std::array<int, 8>, 8>& board, const PiecePosition& from, const PiecePosition& to)
{
    if (from.Row < 0 || from.Row > 7 || from.Col < 0 || from.Col > 7 ||
        to.Row < 0 || to.Row > 7 || to.Col < 0 || to.Col > 7)
        return false;

    int dRow = to.Row - from.Row;
    int dCol = abs(from.Col - to.Col);

    if (dRow == 1 && dCol == 0 && board[to.Row][to.Col] == PIECES_TYPE::EMPTY)
        return true;

    if (from.Row == 1 && dRow == 2 && dCol == 0 && board[to.Row][to.Col] == PIECES_TYPE::EMPTY &&
        board[from.Row + 1][from.Col] == PIECES_TYPE::EMPTY)
        return true;

    if (dRow == 1 && dCol == 1 && board[to.Row][to.Col] > 0)
        return true;

    return false;
}

bool possibleMoveWhitePawn(const std::array<std::array<int, 8>, 8>& board, const PiecePosition& from, const PiecePosition& to)
{
    if (from.Row < 0 || from.Row > 7 || from.Col < 0 || from.Col > 7 ||
        to.Row < 0 || to.Row > 7 || to.Col < 0 || to.Col > 7)
        return false;

    int dRow = from.Row - to.Row;
    int dCol = abs(from.Col - to.Col);

    if (dRow == 1 && dCol == 0 && board[to.Row][to.Col] == PIECES_TYPE::EMPTY)
        return true;

    if (from.Row == 6 && dRow == 2 && dCol == 0 && board[to.Row][to.Col] == PIECES_TYPE::EMPTY &&
        board[from.Row - 1][from.Col] == PIECES_TYPE::EMPTY)
        return true;

    if (dRow == 1 && dCol == 1 && board[to.Row][to.Col] < 0)
        return true;

    return false;
}

bool possibleMoveRook(const std::array<std::array<int, 8>, 8>& board, const PiecePosition& from, const PiecePosition& to)
{
    if (from.Row < 0 || from.Row > 7 || from.Col < 0 || from.Col > 7 ||
        to.Row < 0 || to.Row > 7 || to.Col < 0 || to.Col > 7)
        return false;

    if (from.Col == to.Col)
    {
        if (from.Row < to.Row)
        {
            for (int i = from.Row + 1; i < to.Row; i++)
                if (board[i][from.Col] != PIECES_TYPE::EMPTY)
                    return false;
        }
        else
        {
            for (int i = from.Row - 1; i > to.Row; i--)
                if (board[i][from.Col] != PIECES_TYPE::EMPTY)
                    return false;
        }
        return true;
    }
    else if (from.Row == to.Row)
    {
        if (from.Col < to.Col)
        {
            for (int i = from.Col + 1; i < to.Col; i++)
                if (board[from.Row][i] != PIECES_TYPE::EMPTY)
                    return false;
        }
        else
        {
            for (int i = from.Col - 1; i > to.Col; i--)
                if (board[from.Row][i] != PIECES_TYPE::EMPTY)
                    return false;
        }
        return true;
    }
    return false;
}

bool possibleMoveBishop(const std::array<std::array<int, 8>, 8>& board, const PiecePosition& from, const PiecePosition& to)
{
    if (from.Row < 0 || from.Row > 7 || from.Col < 0 || from.Col > 7 ||
        to.Row < 0 || to.Row > 7 || to.Col < 0 || to.Col > 7)
        return false;

    int dRow = to.Row - from.Row;
    int dCol = to.Col - from.Col;

    if (abs(dRow) != abs(dCol))
        return false;

    int stepRow = (dRow > 0) ? 1 : -1;
    int stepCol = (dCol > 0) ? 1 : -1;

    int currRow = from.Row + stepRow;
    int currCol = from.Col + stepCol;
    while (currRow != to.Row && currCol != to.Col)
    {
        if (board[currRow][currCol] != PIECES_TYPE::EMPTY)
            return false;
        currRow += stepRow;
        currCol += stepCol;
    }
    return true;
}

bool possibleMoveQueen(const std::array<std::array<int, 8>, 8>& board, const PiecePosition& from, const PiecePosition& to)
{
    if (from.Row < 0 || from.Row > 7 || from.Col < 0 || from.Col > 7 ||
        to.Row < 0 || to.Row > 7 || to.Col < 0 || to.Col > 7)
        return false;

    return possibleMoveRook(board, from, to) || possibleMoveBishop(board, from, to);
}

bool possibleMoveKnight(const PiecePosition& from, const PiecePosition& to)
{
    if (to.Row < 0 || to.Row > 7 || to.Col < 0 || to.Col > 7)
        return false;

    int dRow = abs(to.Row - from.Row);
    int dCol = abs(to.Col - from.Col);

    return (dRow == 2 && dCol == 1) || (dRow == 1 && dCol == 2);
}

bool Move::ValidMove(const std::array<std::array<int, 8>, 8>& board, PiecePosition from, PiecePosition to, const std::vector<MoveInfo>& moveHistory)
{
    bool _ValidMove = false;
    int _PIECETYPE = board[from.Row][from.Col];

    switch (_PIECETYPE)
    {
    case PIECES_TYPE::BLACK_PAWN:
        _ValidMove = possibleMoveBlackPawn(board, from, to);
        break;
    case PIECES_TYPE::WHITE_PAWN:
        _ValidMove = possibleMoveWhitePawn(board, from, to);
        break;
    case PIECES_TYPE::BLACK_KING:
        _ValidMove = possibleMoveBlackKing(board, from, to, moveHistory);
        break;
    case PIECES_TYPE::WHITE_KING:
        _ValidMove = possibleMoveWhiteKing(board, from, to, moveHistory);
        break;
    case PIECES_TYPE::WHITE_ROOK:
    case PIECES_TYPE::BLACK_ROOK:
        _ValidMove = possibleMoveRook(board, from, to);
        break;
    case PIECES_TYPE::WHITE_BISHOP:
    case PIECES_TYPE::BLACK_BISHOP:
        _ValidMove = possibleMoveBishop(board, from, to);
        break;
    case PIECES_TYPE::WHITE_QUEEN:
    case PIECES_TYPE::BLACK_QUEEN:
        _ValidMove = possibleMoveQueen(board, from, to);
        break;
    case PIECES_TYPE::WHITE_KNIGHT:
    case PIECES_TYPE::BLACK_KNIGHT:
        _ValidMove = possibleMoveKnight(from, to);
        break;
    }

    if (!_ValidMove) return false;

    if ((_PIECETYPE > 0 && isWhite(board, to)) || (_PIECETYPE < 0 && isBlack(board, to)))
    {
        return false;
    }

    return true;
}

/*
bool Move::isGameOver(const std::array<std::array<int, 8>, 8>& board)
    return false;
}

PieceColor Move::getWinner(const std::array<std::array<int, 8>, 8>& board)
{
    return PieceColor();
}
*/
