#include "move.h"
#include "piecesdefs.h"
#include "pieces.h"
#include <iostream>
static bool isBlack(std::array<std::array<int, MAX_PIECES_LINE>, MAX_PIECES_LINE> p_PieceBoard , PiecePosition p_NewPosition)
{
    return (0 > p_PieceBoard[p_NewPosition.Row][p_NewPosition.Col]) ? true : false;
}
static bool isWhite(std::array<std::array<int, MAX_PIECES_LINE>, MAX_PIECES_LINE> p_PieceBoard , PiecePosition p_NewPosition)
{
    return (0 < p_PieceBoard[p_NewPosition.Row][p_NewPosition.Col]) ? true : false;
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
//pawn
bool possibleMoveBlackPawn(std::array<std::array<int, MAX_PIECES_LINE>, MAX_PIECES_LINE> p_PieceBoard,const PiecePosition StartPos, const PiecePosition EndPos)
{

    if (StartPos.Row < 0 || StartPos.Row > 7 || StartPos.Col < 0 || StartPos.Col > 7 ||
        EndPos.Row < 0 || EndPos.Row > 7 || EndPos.Col < 0 || EndPos.Col > 7)
    {
        return false;
    }
    if (StartPos.Row + 1 == EndPos.Row && StartPos.Col == EndPos.Col)
    {
        if (p_PieceBoard[EndPos.Row][EndPos.Col] == PIECES_TYPE::EMPTY)
            return true;
    }
    else if (StartPos.Row == 1 && StartPos.Col == EndPos.Col)
    {
        if ( EndPos.Row == 3)
        {
            if (p_PieceBoard[EndPos.Row][EndPos.Col] == PIECES_TYPE::EMPTY)
                return true;
        }
    }
    else if ((StartPos.Col - 1 == EndPos.Col || StartPos.Col + 1 == EndPos.Col) && (StartPos.Row + 1 == EndPos.Row))
    {
        if (p_PieceBoard[EndPos.Row][EndPos.Col] != PIECES_TYPE::EMPTY)
            return true;
    }
    return false;
}

 bool possibleMoveWhitePawn(std::array<std::array<int, MAX_PIECES_LINE>, MAX_PIECES_LINE> p_PieceBoard,const PiecePosition StartPos, const PiecePosition EndPos)
{
     if (StartPos.Row < 0 || StartPos.Row > 7 || StartPos.Col < 0 || StartPos.Col > 7 ||
         EndPos.Row < 0 || EndPos.Row > 7 || EndPos.Col < 0 || EndPos.Col > 7)
     {
         return false;
     }
     if (StartPos.Row - 1 == EndPos.Row && StartPos.Col == EndPos.Col)
     {
         if (p_PieceBoard[EndPos.Row][EndPos.Col] == PIECES_TYPE::EMPTY)
             return true;
     }
     else if (StartPos.Row == 6 && StartPos.Col == EndPos.Col)
     {
         if (EndPos.Row == 4)
         {
             if (p_PieceBoard[EndPos.Row][EndPos.Col] == PIECES_TYPE::EMPTY)
                 return true;
         }
     }
     else if ((StartPos.Col - 1 == EndPos.Col || StartPos.Col + 1 == EndPos.Col) && (StartPos.Row - 1 == EndPos.Row))
     {


             if (p_PieceBoard[EndPos.Row][EndPos.Col] != PIECES_TYPE::EMPTY)
                 return true;

     }
     return false;

}
//king
bool possibleMoveBlackKing(const std::array<std::array<int, MAX_PIECES_LINE>, MAX_PIECES_LINE>& p_PieceBoard, const PiecePosition& StartPos, const PiecePosition& EndPos, const std::vector<MoveInfo>& moveHistory)
{
    if (StartPos.Row < 0 || StartPos.Row > 7 || StartPos.Col < 0 || StartPos.Col > 7
        || EndPos.Row < 0 || EndPos.Row > 7 || EndPos.Col < 0 || EndPos.Col > 7)
    {
        return false;
    }

    int dRow = EndPos.Row - StartPos.Row;
    int dCol = EndPos.Col - StartPos.Col;
    if (abs(dRow) > 1 || abs(dCol) > 1)
    {
        return false;
    }

    int pieceAtStart = p_PieceBoard[StartPos.Row][StartPos.Col];
    int pieceAtEnd = p_PieceBoard[EndPos.Row][EndPos.Col];
    if (pieceAtStart >= 0 || (pieceAtEnd < 0))
    {
        return false;
    }

    auto boardAfterMove = p_PieceBoard;
    boardAfterMove[EndPos.Row][EndPos.Col] = boardAfterMove[StartPos.Row][StartPos.Col];
    boardAfterMove[StartPos.Row][StartPos.Col] = PIECES_TYPE::EMPTY;

    if (Move::isBlackKingInCheck(moveHistory, boardAfterMove, EndPos.Row, EndPos.Col))
    {
        return false;
    }

    return true;
}

bool possibleMoveWhiteKing(const std::array<std::array<int, MAX_PIECES_LINE>, MAX_PIECES_LINE>& p_PieceBoard, const PiecePosition& StartPos, const PiecePosition& EndPos, const std::vector<MoveInfo>& moveHistory)
{
    if (StartPos.Row < 0 || StartPos.Row > 7 || StartPos.Col < 0 || StartPos.Col > 7
        || EndPos.Row < 0 || EndPos.Row > 7 || EndPos.Col < 0 || EndPos.Col > 7)
    {
        return false;
    }

    int dRow = EndPos.Row - StartPos.Row;
    int dCol = EndPos.Col - StartPos.Col;
    if (abs(dRow) > 1 || abs(dCol) > 1)
    {
        return false;
    }

    int pieceAtStart = p_PieceBoard[StartPos.Row][StartPos.Col];
    int pieceAtEnd = p_PieceBoard[EndPos.Row][EndPos.Col];
    if (pieceAtStart <= 0 || (pieceAtEnd > 0))
    {
        return false;
    }

    auto boardAfterMove = p_PieceBoard;
    boardAfterMove[EndPos.Row][EndPos.Col] = boardAfterMove[StartPos.Row][StartPos.Col];
    boardAfterMove[StartPos.Row][StartPos.Col] = PIECES_TYPE::EMPTY;

    if (Move::isWhiteKingInCheck(moveHistory, boardAfterMove, EndPos.Row, EndPos.Col))
    {
        return false;
    }

    return true;
}

//rook
static bool possibleMoveRook(std::array<std::array<int, MAX_PIECES_LINE>, MAX_PIECES_LINE> p_PieceBoard,const PiecePosition StartPos, const PiecePosition EndPos)
{
    if (StartPos.Row < 0 || StartPos.Row > 7 || StartPos.Col < 0 || StartPos.Col > 7 ||
        EndPos.Row < 0 || EndPos.Row > 7 || EndPos.Col < 0 || EndPos.Col > 7)
    {
        return false;
    }
    if (StartPos.Col == EndPos.Col)
    {
        for (int i = StartPos.Row - 1; i >= 0; i--)
        {
            if (i == EndPos.Row)
            {
                if (p_PieceBoard[i][StartPos.Col] != PIECES_TYPE::EMPTY)
                    return true;
                return true;
            }
            if (p_PieceBoard[i][StartPos.Col] != PIECES_TYPE::EMPTY)
            {
                break;
            }

        }
        for (int i = StartPos.Row + 1; i <= 7; i++)
        {
            if (i == EndPos.Row)
            {
                if (p_PieceBoard[i][StartPos.Col] != PIECES_TYPE::EMPTY)
                    return true;
                return true;
            }
            if (p_PieceBoard[i][StartPos.Col] != PIECES_TYPE::EMPTY)
            {
                break;
            }
        }
    }
    if (StartPos.Row == EndPos.Row)
    {
        for (int i = StartPos.Col - 1 ; i >= 0; i--)
        {
            if (i == EndPos.Col)
            {
                if (p_PieceBoard[StartPos.Row][i] != PIECES_TYPE::EMPTY)
                    return true;
                return true;
            }
            if (p_PieceBoard[StartPos.Row][i] != PIECES_TYPE::EMPTY)
            {
                break;
            }
        }
        for (int i = StartPos.Col + 1; i <= 7; i++)
        {
            if (i == EndPos.Col)
            {
                if (p_PieceBoard[StartPos.Row][i] != PIECES_TYPE::EMPTY)
                    return true;
                return true;
            }
            if (p_PieceBoard[StartPos.Row][i] != PIECES_TYPE::EMPTY)
            {
                break;
            }
        }
    }
    return false;
}

//bishop
static bool possibleMoveBishop(std::array<std::array<int, MAX_PIECES_LINE>, MAX_PIECES_LINE> p_PieceBoard,const PiecePosition StartPos, const PiecePosition EndPos)
{
    if (StartPos.Row < 0 || StartPos.Row > 7 || StartPos.Col < 0 || StartPos.Col > 7 ||
        EndPos.Row < 0 || EndPos.Row > 7 || EndPos.Col < 0 || EndPos.Col > 7)
    {
        return false;
    }
    int _row = 0, _col = 0;
    _row = StartPos.Row - 1;
    _col = StartPos.Col - 1;
    while (_row >= 0 && _col >= 0)
    {
        if (_row == EndPos.Row && _col == EndPos.Col)
        {
            if (p_PieceBoard[_row][_col] != PIECES_TYPE::EMPTY)
                return true;
            return true;
        }
            if (p_PieceBoard[_row][_col] != PIECES_TYPE::EMPTY)
            {
                break;
            }
            _row--; _col--;

    }
    _row = StartPos.Row + 1;
    _col = StartPos.Col + 1;
    while (_row <= 7 && _col <= 7)
    {
        if (_row == EndPos.Row && _col == EndPos.Col)
        {
            if (p_PieceBoard[_row][_col] != PIECES_TYPE::EMPTY)
                return true;
            return true;
        }
            if (p_PieceBoard[_row][_col] != PIECES_TYPE::EMPTY)
            {
                break;
            }
            _row++; _col++;

    }
    _row = StartPos.Row - 1;
    _col = StartPos.Col + 1;
    while (_row >= 0 && _col <= 7)
    {
        if (_row == EndPos.Row && _col == EndPos.Col)
        {
            if (p_PieceBoard[_row][_col] != PIECES_TYPE::EMPTY)
                return true;
            return true;
        }
        if (p_PieceBoard[_row][_col] != PIECES_TYPE::EMPTY)
        {
            break;
        }
        _row--; _col++;

    }
    _row = StartPos.Row + 1;
    _col = StartPos.Col - 1;
    while (_row <= 7 && _col >= 0)
    {
        if (_row == EndPos.Row && _col == EndPos.Col)
        {
            if (p_PieceBoard[_row][_col] != PIECES_TYPE::EMPTY)
                return true;
            return true;
        }
        if (p_PieceBoard[_row][_col] != PIECES_TYPE::EMPTY)
        {
            break;
        }
        _row++; _col--;

    }
    return false;
}

//queen
bool possibleMoveQueen(std::array<std::array<int, MAX_PIECES_LINE>, MAX_PIECES_LINE> p_PieceBoard,const PiecePosition StartPos, const PiecePosition EndPos)
{
    if (0 <= EndPos.Row && EndPos.Row <= 7 && 0 <= EndPos.Col && EndPos.Col <= 7)
    {
        if (possibleMoveRook(p_PieceBoard,StartPos, EndPos) || possibleMoveBishop(p_PieceBoard, StartPos, EndPos))
        {
            return true;
        }
    }
    return false;
}

//knight
bool possibleMoveKnight(const PiecePosition StartPos, const PiecePosition EndPos)
{
    if (0 <= EndPos.Row && EndPos.Row <= 7 && 0 <= EndPos.Col && EndPos.Col <= 7)
    {
        if (StartPos.Row -1 == EndPos.Row && StartPos.Col + -2 == EndPos.Col)
        {
            return true;
        }
        else if (StartPos.Row + 1 == EndPos.Row && StartPos.Col - 2 == EndPos.Col)
        {
            return true;
        }
        else if (StartPos.Row + 2 == EndPos.Row && StartPos.Col - 1 == EndPos.Col)
        {
            return true;
        }
        else if (StartPos.Row - 2 == EndPos.Row && StartPos.Col - 1 == EndPos.Col)
        {
            return true;
        }
        else if (StartPos.Row - 2 == EndPos.Row && StartPos.Col + 1 == EndPos.Col)
        {
            return true;
        }
        else if (StartPos.Row - 1 == EndPos.Row && StartPos.Col + 2 == EndPos.Col)
        {
            return true;
        }
        else if (StartPos.Row + 1 == EndPos.Row && StartPos.Col + 2 == EndPos.Col)
        {
            return true;
        }
        else if (StartPos.Row + 2 == EndPos.Row && StartPos.Col + 1 == EndPos.Col)
        {
            return true;
        }
    }
    return false;
}

bool Move::ValidMove(std::array<std::array<int, MAX_PIECES_LINE>, MAX_PIECES_LINE> p_PieceBoard, PiecePosition p_OldPosition, PiecePosition p_NewPosition, const std::vector<MoveInfo>& moveHistory)
{
    bool _ValidMove = false;
    int _PIECETYPE = p_PieceBoard[p_OldPosition.Row][p_OldPosition.Col];

    switch (_PIECETYPE)
    {
    case PIECES_TYPE::BLACK_PAWN:
        _ValidMove = possibleMoveBlackPawn(p_PieceBoard, p_OldPosition, p_NewPosition);
        break;
    case PIECES_TYPE::WHITE_PAWN:
        _ValidMove = possibleMoveWhitePawn(p_PieceBoard, p_OldPosition, p_NewPosition);
        break;
    case PIECES_TYPE::BLACK_KING:
        _ValidMove = possibleMoveBlackKing(p_PieceBoard, p_OldPosition, p_NewPosition, moveHistory);
        break;
    case PIECES_TYPE::WHITE_KING:
        _ValidMove = possibleMoveWhiteKing(p_PieceBoard, p_OldPosition, p_NewPosition, moveHistory);
        break;
    case PIECES_TYPE::WHITE_ROOK:
    case PIECES_TYPE::BLACK_ROOK:
        _ValidMove = possibleMoveRook(p_PieceBoard, p_OldPosition, p_NewPosition);
        break;
    case PIECES_TYPE::WHITE_BISHOP:
    case PIECES_TYPE::BLACK_BISHOP:
        _ValidMove = possibleMoveBishop(p_PieceBoard, p_OldPosition, p_NewPosition);
        break;
    case PIECES_TYPE::WHITE_QUEEN:
    case PIECES_TYPE::BLACK_QUEEN:
        _ValidMove = possibleMoveQueen(p_PieceBoard, p_OldPosition, p_NewPosition);
        break;
    case PIECES_TYPE::WHITE_KNIGHT:
    case PIECES_TYPE::BLACK_KNIGHT:
        _ValidMove = possibleMoveKnight(p_OldPosition, p_NewPosition);
        break;
    }

    if (!_ValidMove) return false;

    if ((_PIECETYPE > 0 && isWhite(p_PieceBoard, p_NewPosition)) ||
        (_PIECETYPE < 0 && isBlack(p_PieceBoard, p_NewPosition)))
    {
        return false;
    }

    return true;
}

/*
bool Move::isGameOver(std::array<std::array<int, MAX_PIECES_LINE>, MAX_PIECES_LINE> p_PieceBoard)
{
    return false;
}

PieceColor Move::getWinner(std::array<std::array<int, MAX_PIECES_LINE>, MAX_PIECES_LINE> p_PieceBoard)
{
    return PieceColor();
}
*/
