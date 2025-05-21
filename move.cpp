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
    if (startX < 0 || startX >= 8 || startY < 0 || startY >= 8 ||
        endX < 0 || endX >= 8 || endY < 0 || endY >= 8) {
        return;
    }
    moveHistory.push_back({startX, startY, endX, endY, pieceMoved, pieceCaptured});
}
const std::vector<MoveInfo>& Move::getMoveHistory() const {
    return moveHistory;
}

bool Move::isBlackKingInCheck(const std::array<std::array<int, 8>, 8>& board, const std::vector<MoveInfo>& moveHistory, int blackKingX, int blackKingY) {
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
bool Move::isWhiteKingInCheck(const std::array<std::array<int, 8>, 8>& board, const std::vector<MoveInfo>& moveHistory, int whiteKingX, int whiteKingY) {
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
bool Move::isPinned(const std::array<std::array<int, 8>, 8>& board, const std::vector<MoveInfo>& moveHistory, int pieceX, int pieceY)
{
    int piece = board[pieceX][pieceY];
    if (piece == 0) return false;

    int kingX = -1, kingY = -1;
    for (int r = 0; r < 8; ++r)
    {
        for (int c = 0; c < 8; ++c)
        {
            if ((piece > 0 && board[r][c] == PIECES_TYPE::WHITE_KING) ||
                (piece < 0 && board[r][c] == PIECES_TYPE::BLACK_KING))
            {
                kingX = r;
                kingY = c;
                break;
            }
        }
    }

    if (kingX == -1 || kingY == -1)
        return false;

    std::array<std::array<int, 8>, 8> tempBoard = board;
    tempBoard[pieceX][pieceY] = 0;

    if (piece > 0 && isWhiteKingInCheck(tempBoard, moveHistory, kingX, kingY))
        return true;
    if (piece < 0 && isBlackKingInCheck(tempBoard, moveHistory, kingX, kingY))
        return true;

    return false;
}

std::vector<PiecePosition> Move::LegalMoves(const std::array<std::array<int, 8>, 8>& board, const std::vector<MoveInfo>& moveHistory, int kingX, int kingY, int pieceX, int pieceY, bool isWhite)
{
    std::vector<PiecePosition> legalMoves;
    int piece = board[pieceX][pieceY];

    if (isPinned(board, moveHistory, pieceX, pieceY))
    {
        int dx[] = {1, -1, 0, 0, 1, -1, 1, -1};
        int dy[] = {0, 0, 1, -1, 1, -1, -1, 1};

        for (int dir = 0; dir < 8; ++dir)
        {
            for (int step = 1; step <= 7; ++step)
            {
                int nx = pieceX + dx[dir] * step;
                int ny = pieceY + dy[dir] * step;

                if (nx < 0 || ny < 0 || nx > 7 || ny > 7)
                    break;

                if (Move::ValidMove(board, PiecePosition(pieceX, pieceY), PiecePosition(nx, ny), moveHistory))
                {
                    std::array<std::array<int, 8>, 8> tempBoard = board;
                    tempBoard[pieceX][pieceY] = 0;
                    tempBoard[nx][ny] = piece;

                    if (isWhite && !isWhiteKingInCheck(tempBoard, moveHistory, kingX, kingY))
                        legalMoves.push_back(PiecePosition(nx, ny));
                    else if (!isWhite && !isBlackKingInCheck(tempBoard, moveHistory, kingX, kingY))
                        legalMoves.push_back(PiecePosition(nx, ny));
                }

                if (board[nx][ny] != 0)
                    break;
            }
        }
    }
    else
    {
        for (int r = 0; r <= 7; ++r)
        {
            for (int c = 0; c <= 7; ++c)
            {
                if (Move::ValidMove(board, PiecePosition(pieceX, pieceY), PiecePosition(r, c), moveHistory))
                {
                    std::array<std::array<int, 8>, 8> tempBoard = board;
                    tempBoard[pieceX][pieceY] = 0;
                    tempBoard[r][c] = piece;

                    if (isWhite && !isWhiteKingInCheck(tempBoard, moveHistory, kingX, kingY))
                        legalMoves.push_back(PiecePosition(r, c));
                    else if (!isWhite && !isBlackKingInCheck(tempBoard, moveHistory, kingX, kingY))
                        legalMoves.push_back(PiecePosition(r, c));
                }
            }
        }
    }

    return legalMoves;
}
bool Move::canBlockCheck(const std::vector<MoveInfo>& moveHistory, const std::array<std::array<int, 8>, 8>& board, int kingX, int kingY, int pieceX, int pieceY, bool isWhite)
{
    auto legalMoves = Move::LegalMoves(board, moveHistory, kingX, kingY, pieceX, pieceY, isWhite);
    return !legalMoves.empty();
}
bool Move::isCheckmate(const std::array<std::array<int, 8>, 8>& board, const std::vector<MoveInfo>& moveHistory, int kingX, int kingY, bool isWhite)
{
    if (!(isWhite ? isWhiteKingInCheck(board, moveHistory, kingX, kingY)
                  : isBlackKingInCheck(board, moveHistory, kingX, kingY)))
        return false;

    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) continue;

            int newX = kingX + dx;
            int newY = kingY + dy;

            if (newX >= 0 && newX <= 7 && newY >= 0 && newY <= 7) {
                int targetPiece = board[newX][newY];

                if ((isWhite && targetPiece > 0) || (!isWhite && targetPiece < 0))
                    continue;
                std::array<std::array<int, 8>, 8> tempBoard = board;
                tempBoard[kingX][kingY] = 0;
                tempBoard[newX][newY] = board[kingX][kingY];

                if (!(isWhite ? isWhiteKingInCheck(tempBoard, moveHistory, newX, newY)
                              : isBlackKingInCheck(tempBoard, moveHistory, newX, newY))) {
                    return false;
                }
            }
        }
    }

    for (int row = 0; row <= 7; row++) {
        for (int col = 0; col <= 7; col++) {
            int piece = board[row][col];
            if ((isWhite && piece > 0) || (!isWhite && piece < 0)) {
                auto legalMoves = Move::LegalMoves(board, moveHistory, kingX, kingY, row, col, isWhite);

                for (const auto& move : legalMoves) {
                    std::array<std::array<int, 8>, 8> tempBoard = board;
                    tempBoard[move.Row][move.Col] = piece;
                    tempBoard[row][col] = 0;

                    if (!(isWhite ? isWhiteKingInCheck(tempBoard, moveHistory, kingX, kingY)
                                  : isBlackKingInCheck(tempBoard, moveHistory, kingX, kingY))) {
                        return false;
                    }
                }
            }
        }
    }

    return true;
}
bool Move::isStalemate(const std::array<std::array<int, 8>, 8>& board, const std::vector<MoveInfo>& moveHistory, int kingX, int kingY, bool isWhite) {
    if (isWhite ? isWhiteKingInCheck(board, moveHistory, kingX, kingY)
                : isBlackKingInCheck(board, moveHistory, kingX, kingY)) {
        return false;
    }
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (dx == 0 && dy == 0) continue;

            int newX = kingX + dx;
            int newY = kingY + dy;

            if (newX >= 0 && newX < 8 && newY >= 0 && newY < 8) {
                int targetPiece = board[newX][newY];

                if ((isWhite && targetPiece > 0) || (!isWhite && targetPiece < 0))
                    continue;

                std::array<std::array<int, 8>, 8> tempBoard = board;
                tempBoard[kingX][kingY] = 0;
                tempBoard[newX][newY] = board[kingX][kingY];

                if (!(isWhite ? isWhiteKingInCheck(tempBoard, moveHistory, newX, newY)
                              : isBlackKingInCheck(tempBoard, moveHistory, newX, newY))) {
                    return false;
                }
            }
        }
    }

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            int piece = board[row][col];
            if ((isWhite && piece > 0) || (!isWhite && piece < 0)) {
                auto legalMoves = Move::LegalMoves(board, moveHistory, kingX, kingY, row, col, isWhite);

                for (const auto& move : legalMoves) {
                    std::array<std::array<int, 8>, 8> tempBoard = board;
                    tempBoard[move.Row][move.Col] = piece;
                    tempBoard[row][col] = 0;

                    if (!(isWhite ? isWhiteKingInCheck(tempBoard, moveHistory, kingX, kingY)
                                  : isBlackKingInCheck(tempBoard, moveHistory, kingX, kingY))) {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}
bool Move::isInsufficientMaterial(const std::array<std::array<int, 8>, 8>& board)
{
    int whitePieceCount = 0, blackPieceCount = 0;
    int whiteKnightCount = 0, blackKnightCount = 0;
    int whiteBishopCount = 0, blackBishopCount = 0;

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            int piece = board[row][col];

            if (piece == PIECES_TYPE::WHITE_KING || piece == PIECES_TYPE::BLACK_KING)
                continue;

            if (piece > 0) {
                whitePieceCount++;
                if (piece == PIECES_TYPE::WHITE_KNIGHT) whiteKnightCount++;
                if (piece == PIECES_TYPE::WHITE_BISHOP) whiteBishopCount++;
            } else if (piece < 0) {
                blackPieceCount++;
                if (piece == PIECES_TYPE::BLACK_KNIGHT) blackKnightCount++;
                if (piece == PIECES_TYPE::BLACK_BISHOP) blackBishopCount++;
            }
        }
    }

    return (whitePieceCount == 0 && blackPieceCount == 0) ||
           (whitePieceCount == 1 && blackPieceCount == 0 && (whiteKnightCount == 1 || whiteBishopCount == 1)) ||
           (blackPieceCount == 1 && whitePieceCount == 0 && (blackKnightCount == 1 || blackBishopCount == 1)) ||
           (whitePieceCount == 2 && blackPieceCount == 0 && whiteKnightCount == 2) ||
           (blackPieceCount == 2 && whitePieceCount == 0 && blackKnightCount == 2);
}
bool Move::canCastle(const std::array<std::array<int, 8>, 8>& board, bool isWhite, const std::vector<MoveInfo>& moveHistory)
{
    int row = isWhite ? 7 : 0;
    PiecePosition kingPos{row, 4};
    PiecePosition rookKingside{row, 7};
    PiecePosition rookQueenside{row, 0};
    if (board[row][4] != (isWhite ? PIECES_TYPE::WHITE_KING : PIECES_TYPE::BLACK_KING))
        return false;

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
        if ((isWhite && Move::isWhiteKingInCheck(board, moveHistory, row, col)) || (!isWhite && Move::isBlackKingInCheck(board, moveHistory, row, col)))
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
        if ((isWhite && Move::isWhiteKingInCheck(board, moveHistory, row, col)) ||
            (!isWhite && Move::isBlackKingInCheck(board, moveHistory, row, col)))
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

    if (pieceAtStart != PIECES_TYPE::BLACK_KING || (pieceAtEnd < 0))
        return false;

    auto tempBoard = board;
    tempBoard[to.Row][to.Col] = pieceAtStart;
    tempBoard[from.Row][from.Col] = PIECES_TYPE::EMPTY;

    if (Move::isBlackKingInCheck(tempBoard, moveHistory, to.Row, to.Col))
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

    if ((pieceAtStart != PIECES_TYPE::WHITE_KING) || (pieceAtEnd > 0))
        return false;

    auto tempBoard = board;
    tempBoard[to.Row][to.Col] = pieceAtStart;
    tempBoard[from.Row][from.Col] = PIECES_TYPE::EMPTY;

    if (Move::isWhiteKingInCheck(tempBoard, moveHistory, to.Row, to.Col))
        return false;

    return true;
}
bool Move::canEnPassantLeft(const std::array<std::array<int, 8>, 8>& board, bool isWhite, const std::vector<MoveInfo>& moveHistory, int pawnRow, int pawnCol) {
    if (moveHistory.empty() || pawnCol <= 0)
        return false;

    const MoveInfo& lastMove = moveHistory.back();
    int enemyPawn = isWhite ? PIECES_TYPE::BLACK_PAWN : PIECES_TYPE::WHITE_PAWN;
    int startRow = isWhite ? 1 : 6;
    int targetRow = isWhite ? 3 : 4;

    return lastMove.pieceMoved == enemyPawn && lastMove.startX == startRow && lastMove.endX == targetRow &&
           lastMove.endY == pawnCol - 1 && pawnRow == targetRow;
}

bool Move::canEnPassantRight(const std::array<std::array<int, 8>, 8>& board, bool isWhite, const std::vector<MoveInfo>& moveHistory, int pawnRow, int pawnCol) {
    if (moveHistory.empty() || pawnCol >= 7)
        return false;

    const MoveInfo& lastMove = moveHistory.back();
    int enemyPawn = isWhite ? PIECES_TYPE::BLACK_PAWN : PIECES_TYPE::WHITE_PAWN;
    int startRow = isWhite ? 1 : 6;
    int targetRow = isWhite ? 3 : 4;

    return lastMove.pieceMoved == enemyPawn && lastMove.startX == startRow && lastMove.endX == targetRow &&
           lastMove.endY == pawnCol + 1 && pawnRow == targetRow;
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
