#include "board.h"
#include "colordefs.h"
#include <iostream>

static bool round(int num)
{
    return (num > 0);
}
void Board::BlackPawnHint()
{
    if (Move::isPinned(m_Player.BoardPieces, m_MoveHistory, m_Player.row, m_Player.col))
        return;

    if (!(0 <= m_Player.row && m_Player.row <= 7 && 0 <= m_Player.col && m_Player.col <= 7))
        return;

    if (m_Player.BoardPieces[m_Player.row + 1][m_Player.col] == PIECES_TYPE::EMPTY) {
        SetHighlight(m_Player.row + 1, m_Player.col);
        if (m_Player.row == 1 && m_Player.BoardPieces[m_Player.row + 2][m_Player.col] == PIECES_TYPE::EMPTY) {
            SetHighlight(m_Player.row + 2, m_Player.col);
        }
    }

    if (m_Player.col > 0 && m_Player.BoardPieces[m_Player.row + 1][m_Player.col - 1] > 0) {
        SetHighlight(m_Player.row + 1, m_Player.col - 1);
    }

    if (m_Player.col < 7 && m_Player.BoardPieces[m_Player.row + 1][m_Player.col + 1] > 0) {
        SetHighlight(m_Player.row + 1, m_Player.col + 1);
    }

    if (Move::canEnPassantLeft(m_Player.BoardPieces, false, m_MoveHistory, m_Player.row, m_Player.col)) {
        SetHighlight(m_Player.row + 1, m_Player.col - 1);
    }

    if (Move::canEnPassantRight(m_Player.BoardPieces, false, m_MoveHistory, m_Player.row, m_Player.col)) {
        SetHighlight(m_Player.row + 1, m_Player.col + 1);
    }
}
void Board::WhitePawnHint()
{
    if (Move::isPinned(m_Player.BoardPieces, m_MoveHistory, m_Player.row, m_Player.col))
        return;

    if (!(0 <= m_Player.row && m_Player.row <= 7 && 0 <= m_Player.col && m_Player.col <= 7))
        return;

    if (m_Player.BoardPieces[m_Player.row - 1][m_Player.col] == PIECES_TYPE::EMPTY) {
        SetHighlight(m_Player.row - 1, m_Player.col);
        if (m_Player.row == 6 && m_Player.BoardPieces[m_Player.row - 2][m_Player.col] == PIECES_TYPE::EMPTY) {
            SetHighlight(m_Player.row - 2, m_Player.col);
        }
    }
    if (m_Player.col > 0 && m_Player.BoardPieces[m_Player.row - 1][m_Player.col - 1] < 0) {
        SetHighlight(m_Player.row - 1, m_Player.col - 1);
    }
    if (m_Player.col < 7 && m_Player.BoardPieces[m_Player.row - 1][m_Player.col + 1] < 0) {
        SetHighlight(m_Player.row - 1, m_Player.col + 1);
    }

    if (Move::canEnPassantLeft(m_Player.BoardPieces, true, m_MoveHistory, m_Player.row, m_Player.col)) {
        SetHighlight(m_Player.row - 1, m_Player.col - 1);
    }

    if (Move::canEnPassantRight(m_Player.BoardPieces, true, m_MoveHistory, m_Player.row, m_Player.col)) {
        SetHighlight(m_Player.row - 1, m_Player.col + 1);
    }
}
void Board::BlackKingHint()
{
    if (!(0 <= m_Player.row && m_Player.row <= 7 && 0 <= m_Player.col && m_Player.col <= 7))
        return;

    auto tempBoard = m_Player.BoardPieces;

    if (Move::isBlackKingInCheck(tempBoard, moveManager.getMoveHistory(), m_Player.row, m_Player.col))
    {
        showPieceHightlight(m_Player.row, m_Player.col);
    }

    for (int i = m_Player.row - 1; i <= m_Player.row + 1; i++)
    {
        for (int j = m_Player.col - 1; j <= m_Player.col + 1; j++)
        {
            if (i >= 0 && i <= 7 && j >= 0 && j <= 7)
            {
                if (i == m_Player.row && j == m_Player.col) continue;

                int targetPiece = tempBoard[i][j];

                if (targetPiece < 0) continue;

                int originalPiece = tempBoard[i][j];
                tempBoard[i][j] = tempBoard[m_Player.row][m_Player.col];
                tempBoard[m_Player.row][m_Player.col] = PIECES_TYPE::EMPTY;

                if (!Move::isBlackKingInCheck(tempBoard, moveManager.getMoveHistory(), i, j))
                {
                    SetHighlight(i, j);
                }

                tempBoard[m_Player.row][m_Player.col] = tempBoard[i][j];
                tempBoard[i][j] = originalPiece;
            }
        }
    }
    if (Move::canCastle(tempBoard, false, moveManager.getMoveHistory())) {
        if (tempBoard[0][5] == PIECES_TYPE::EMPTY && tempBoard[0][6] == PIECES_TYPE::EMPTY &&
            tempBoard[0][7] == PIECES_TYPE::BLACK_ROOK) {
            SetHighlight(0, 6);
        }

        if (tempBoard[0][1] == PIECES_TYPE::EMPTY && tempBoard[0][2] == PIECES_TYPE::EMPTY &&
            tempBoard[0][3] == PIECES_TYPE::EMPTY && tempBoard[0][0] == PIECES_TYPE::BLACK_ROOK) {
            SetHighlight(0, 2);
        }
    }
}
void Board::WhiteKingHint()
{
    if (!(0 <= m_Player.row && m_Player.row <= 7 && 0 <= m_Player.col && m_Player.col <= 7))
        return;

    auto tempBoard = m_Player.BoardPieces;

    if (Move::isWhiteKingInCheck(tempBoard, moveManager.getMoveHistory(), m_Player.row, m_Player.col))
    {
        showPieceHightlight(m_Player.row, m_Player.col);
    }

    for (int i = m_Player.row - 1; i <= m_Player.row + 1; i++)
    {
        for (int j = m_Player.col - 1; j <= m_Player.col + 1; j++)
        {
            if (i >= 0 && i <= 7 && j >= 0 && j <= 7)
            {
                if (i == m_Player.row && j == m_Player.col) continue;

                int targetPiece = tempBoard[i][j];

                if (targetPiece > 0) continue;

                int originalPiece = tempBoard[i][j];
                tempBoard[i][j] = tempBoard[m_Player.row][m_Player.col];
                tempBoard[m_Player.row][m_Player.col] = PIECES_TYPE::EMPTY;

                if (!Move::isWhiteKingInCheck(tempBoard, moveManager.getMoveHistory(), i, j))
                {
                    SetHighlight(i, j);
                }

                tempBoard[m_Player.row][m_Player.col] = tempBoard[i][j];
                tempBoard[i][j] = originalPiece;
            }
        }
    }
    if (Move::canCastle(tempBoard, true, moveManager.getMoveHistory())) {
        if (tempBoard[7][5] == PIECES_TYPE::EMPTY && tempBoard[7][6] == PIECES_TYPE::EMPTY &&
            tempBoard[7][7] == PIECES_TYPE::WHITE_ROOK) {
            SetHighlight(7, 6);
        }

        if (tempBoard[7][1] == PIECES_TYPE::EMPTY && tempBoard[7][2] == PIECES_TYPE::EMPTY &&
            tempBoard[7][3] == PIECES_TYPE::EMPTY && tempBoard[7][0] == PIECES_TYPE::WHITE_ROOK) {
            SetHighlight(7, 2);
        }
    }
}
void Board::QueenHint()
{
    BishopHint();
    RookHint();
}
void Board::KnightHint()
{
    if (Move::isPinned(m_Player.BoardPieces, m_MoveHistory, m_Player.row, m_Player.col))
        return;

    if ((0 <= m_Player.row && m_Player.row <= 7) || (0 <= m_Player.col && m_Player.col <= 7))
    {
        int pieceColor = round(m_Player.BoardPieces[m_Player.row][m_Player.col]);

        if (m_Player.row - 1 <= 7 && m_Player.row - 1 >= 0 && m_Player.col - 2 <= 7 && m_Player.col - 2 >= 0)
        {
            if (round(m_Player.BoardPieces[m_Player.row - 1][m_Player.col - 2]) != pieceColor && m_Player.BoardPieces[m_Player.row - 1][m_Player.col - 2] != PIECES_TYPE::EMPTY)
                SetHighlight(m_Player.row - 1, m_Player.col - 2);
            else if (m_Player.BoardPieces[m_Player.row - 1][m_Player.col - 2] == PIECES_TYPE::EMPTY)
            {
                SetHighlight(m_Player.row - 1, m_Player.col - 2);
            }
        }
        if (m_Player.row + 1 <= 7 && m_Player.row + 1 >= 0 && m_Player.col - 2 <= 7 && m_Player.col - 2 >= 0)
        {
            if (round(m_Player.BoardPieces[m_Player.row + 1][m_Player.col - 2]) != pieceColor && m_Player.BoardPieces[m_Player.row + 1][m_Player.col - 2] != PIECES_TYPE::EMPTY)
                SetHighlight(m_Player.row + 1, m_Player.col - 2);
            else if (m_Player.BoardPieces[m_Player.row + 1][m_Player.col - 2] == PIECES_TYPE::EMPTY)
            {
                SetHighlight(m_Player.row + 1, m_Player.col - 2);
            }
        }
        if (m_Player.row + 2 <= 7 && m_Player.row + 2 >= 0 && m_Player.col - 1 <= 7 && m_Player.col - 1 >= 0)
        {
            if (round(m_Player.BoardPieces[m_Player.row + 2][m_Player.col - 1]) != pieceColor && m_Player.BoardPieces[m_Player.row + 2][m_Player.col - 1] != PIECES_TYPE::EMPTY)
                SetHighlight(m_Player.row + 2, m_Player.col - 1);
            else if (m_Player.BoardPieces[m_Player.row + 2][m_Player.col - 1] == PIECES_TYPE::EMPTY)
            {
                SetHighlight(m_Player.row + 2, m_Player.col - 1);
            }
        }
        if (m_Player.row - 2 <= 7 && m_Player.row - 2 >= 0 && m_Player.col - 1 <= 7 && m_Player.col - 1 >= 0)
        {
            if (round(m_Player.BoardPieces[m_Player.row - 2][m_Player.col - 1]) != pieceColor && m_Player.BoardPieces[m_Player.row - 2][m_Player.col - 1] != PIECES_TYPE::EMPTY)
                SetHighlight(m_Player.row - 2, m_Player.col - 1);
            else if (m_Player.BoardPieces[m_Player.row - 2][m_Player.col - 1] == PIECES_TYPE::EMPTY)
            {
                SetHighlight(m_Player.row - 2, m_Player.col - 1);
            }
        }
        if (m_Player.row - 2 <= 7 && m_Player.row - 2 >= 0 && m_Player.col + 1 <= 7 && m_Player.col + 1 >= 0)
        {
            if (round(m_Player.BoardPieces[m_Player.row - 2][m_Player.col + 1]) != pieceColor && m_Player.BoardPieces[m_Player.row - 2][m_Player.col + 1] != PIECES_TYPE::EMPTY)
                SetHighlight(m_Player.row - 2, m_Player.col + 1);
            else if (m_Player.BoardPieces[m_Player.row - 2][m_Player.col + 1] == PIECES_TYPE::EMPTY)
            {
                SetHighlight(m_Player.row - 2, m_Player.col + 1);
            }
        }
        if (m_Player.row - 1 <= 7 && m_Player.row - 1 >= 0 && m_Player.col + 2 <= 7 && m_Player.col + 2 >= 0)
        {
            if (round(m_Player.BoardPieces[m_Player.row - 1][m_Player.col + 2]) != pieceColor && m_Player.BoardPieces[m_Player.row - 1][m_Player.col + 2] != PIECES_TYPE::EMPTY)
                SetHighlight(m_Player.row - 1, m_Player.col + 2);
            else if (m_Player.BoardPieces[m_Player.row - 1][m_Player.col + 2] == PIECES_TYPE::EMPTY)
            {
                SetHighlight(m_Player.row - 1, m_Player.col + 2);
            }
        }
        if (m_Player.row + 1 <= 7 && m_Player.row + 1 >= 0 && m_Player.col + 2 <= 7 && m_Player.col + 2 >= 0)
        {
            if (round(m_Player.BoardPieces[m_Player.row + 1][m_Player.col + 2]) != pieceColor && m_Player.BoardPieces[m_Player.row + 1][m_Player.col + 2] != PIECES_TYPE::EMPTY)
                SetHighlight(m_Player.row + 1, m_Player.col + 2);
            else if (m_Player.BoardPieces[m_Player.row + 1][m_Player.col + 2] == PIECES_TYPE::EMPTY)
            {
                SetHighlight(m_Player.row + 1, m_Player.col + 2);
            }
        }
        if (m_Player.row + 2 <= 7 && m_Player.row + 2 >= 0 && m_Player.col + 1 <= 7 && m_Player.col + 1 >= 0)
        {
            if (round(m_Player.BoardPieces[m_Player.row + 2][m_Player.col + 1]) != pieceColor && m_Player.BoardPieces[m_Player.row + 2][m_Player.col + 1] != PIECES_TYPE::EMPTY)
                SetHighlight(m_Player.row + 2, m_Player.col + 1);
            else if (m_Player.BoardPieces[m_Player.row + 2][m_Player.col + 1] == PIECES_TYPE::EMPTY)
            {
                SetHighlight(m_Player.row + 2, m_Player.col + 1);
            }
        }
    }
}
void Board::RookHint()
{
    if (Move::isPinned(m_Player.BoardPieces, m_MoveHistory, m_Player.row, m_Player.col))
        return;

    if (!(0 <= m_Player.row && m_Player.row <= 7 && 0 <= m_Player.col && m_Player.col <= 7))
        return;

    int pieceColor = round(m_Player.BoardPieces[m_Player.row][m_Player.col]);
    for (int i = m_Player.row - 1; i >= 0; i--)
    {
        if (m_Player.BoardPieces[i][m_Player.col] == PIECES_TYPE::EMPTY)
        {
            SetHighlight(i, m_Player.col);
        }
        if (m_Player.BoardPieces[i][m_Player.col] != PIECES_TYPE::EMPTY)
        {
            if (round(m_Player.BoardPieces[i][m_Player.col]) != pieceColor)
                SetHighlight(i, m_Player.col);
        }
        if (m_Player.BoardPieces[i][m_Player.col] != PIECES_TYPE::EMPTY)
        {
            break;
        }
    }
    for (int i = m_Player.row + 1; i <= 7; i++)
    {
        if (m_Player.BoardPieces[i][m_Player.col] == PIECES_TYPE::EMPTY)
        {
            SetHighlight(i, m_Player.col);
        }
        if (m_Player.BoardPieces[i][m_Player.col] != PIECES_TYPE::EMPTY)
        {
            if (round(m_Player.BoardPieces[i][m_Player.col]) != pieceColor)
                SetHighlight(i, m_Player.col);
        }
        if (m_Player.BoardPieces[i][m_Player.col] != PIECES_TYPE::EMPTY)
        {
            break;
        }
    }

    for (int i = m_Player.col - 1; i >= 0; i--)
    {
        if (m_Player.BoardPieces[m_Player.row][i] == PIECES_TYPE::EMPTY)
        {
            SetHighlight(m_Player.row, i);
        }
        if (m_Player.BoardPieces[m_Player.row][i] != PIECES_TYPE::EMPTY)
        {
            if (round(m_Player.BoardPieces[m_Player.row][i]) != pieceColor)
                SetHighlight(m_Player.row, i);
        }
        if (m_Player.BoardPieces[m_Player.row][i] != PIECES_TYPE::EMPTY)
        {
            break;
        }
    }
    for (int i = m_Player.col + 1; i <= 7; i++)
    {
        if (m_Player.BoardPieces[m_Player.row][i] == PIECES_TYPE::EMPTY)
        {
            SetHighlight(m_Player.row, i);
        }
        if (m_Player.BoardPieces[m_Player.row][i] != PIECES_TYPE::EMPTY)
        {
            if (round(m_Player.BoardPieces[m_Player.row][i]) != pieceColor)
                SetHighlight(m_Player.row, i);
        }
        if (m_Player.BoardPieces[m_Player.row][i] != PIECES_TYPE::EMPTY)
        {
            break;
        }
    }
}
void Board::BishopHint()
{
    if (Move::isPinned(m_Player.BoardPieces, m_MoveHistory, m_Player.row, m_Player.col))
        return;

    if (!(0 <= m_Player.row && m_Player.row <= 7 && 0 <= m_Player.col && m_Player.col <= 7))
        return;

    int pieceColor = round(m_Player.BoardPieces[m_Player.row][m_Player.col]);
    int _row = 0, _col = 0;
    _row = m_Player.row - 1;
    _col = m_Player.col - 1;
    while (_row >= 0 && _col >= 0)
    {
        if (m_Player.BoardPieces[_row][_col] == PIECES_TYPE::EMPTY)
        {
            SetHighlight(_row, _col);
        }
        if (m_Player.BoardPieces[_row][_col] != PIECES_TYPE::EMPTY)
        {
            if (round(m_Player.BoardPieces[_row][_col]) != pieceColor)
            {
                SetHighlight(_row, _col);
            }
            break;
        }
        _row--;
        _col--;
    }
    _row = m_Player.row + 1;
    _col = m_Player.col + 1;
    while (_row <= 7 && _col <= 7)
    {
        if (m_Player.BoardPieces[_row][_col] == PIECES_TYPE::EMPTY)
        {
            SetHighlight(_row, _col);
        }
        if (m_Player.BoardPieces[_row][_col] != PIECES_TYPE::EMPTY)
        {
            if (round(m_Player.BoardPieces[_row][_col]) != pieceColor)
            {
                SetHighlight(_row, _col);
            }
            break;
        }
        _row++;
        _col++;
    }
    _row = m_Player.row - 1;
    _col = m_Player.col + 1;
    while (_row >= 0 && _col <= 7)
    {
        if (m_Player.BoardPieces[_row][_col] == PIECES_TYPE::EMPTY)
        {
            SetHighlight(_row, _col);
        }
        if (m_Player.BoardPieces[_row][_col] != PIECES_TYPE::EMPTY)
        {
            if (round(m_Player.BoardPieces[_row][_col]) != pieceColor)
            {
                SetHighlight(_row, _col);
            }
            break;
        }
        _row--;
        _col++;
    }
    _row = m_Player.row + 1;
    _col = m_Player.col - 1;
    while (_row <= 7 && _col >= 0)
    {
        if (m_Player.BoardPieces[_row][_col] == PIECES_TYPE::EMPTY)
        {
            SetHighlight(_row, _col);
        }
        if (m_Player.BoardPieces[_row][_col] != PIECES_TYPE::EMPTY)
        {
            if (round(m_Player.BoardPieces[_row][_col]) != pieceColor)
            {
                SetHighlight(_row, _col);
            }
            break;
        }
        _row++;
        _col--;
    }
}

Board::Board(int p_SizeBoard, SDL_Renderer* p_Renderer)
    : m_BoardSize(p_SizeBoard), m_Renderer(p_Renderer)
{
    m_PiecesSize = m_BoardSize / 8;
    resetBoardColor();
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

void Board::SetHighlight(unsigned const int row, unsigned const int col) {
    if ((row + col) % 2 == 0) {
        m_BoardColor[row][col] = SDL_Color{0, 255, 0, 255};
    }
    else {
        m_BoardColor[row][col] = SDL_Color{0, 200, 0, 255};
    }
}

void Board::showHints(Player p_Player)
{
    int _pieceType = p_Player.BoardPieces[p_Player.row][p_Player.col];
    PiecePosition _pos(p_Player.row, p_Player.col);
    switch (_pieceType)
    {
    case PIECES_TYPE::BLACK_PAWN:
        BlackPawnHint();
        break;
    case PIECES_TYPE::WHITE_PAWN:
        WhitePawnHint();
        break;
    case PIECES_TYPE::BLACK_KING:
        BlackKingHint();
        break;
    case PIECES_TYPE::WHITE_KING:
        WhiteKingHint();
        break;
    case PIECES_TYPE::BLACK_KNIGHT:
        KnightHint();
        break;
    case PIECES_TYPE::WHITE_KNIGHT:
        KnightHint();
        break;
    case PIECES_TYPE::BLACK_ROOK:
        RookHint();
        break;
    case PIECES_TYPE::WHITE_ROOK:
        RookHint();
        break;
    case PIECES_TYPE::BLACK_BISHOP:
        BishopHint();
        break;
    case PIECES_TYPE::WHITE_BISHOP:
        BishopHint();
        break;
    case PIECES_TYPE::BLACK_QUEEN:
        QueenHint();
        break;
    case PIECES_TYPE::WHITE_QUEEN:
        QueenHint();
        break;
    }
}
void Board::showPieceHightlight(unsigned const int row, unsigned const int col)
{
    m_BoardColor[row][col] = SDL_Color{255, 0, 0, 255};
}

void Board::UpdatePlayer(Player p_Player)
{
    m_Player = p_Player;
    if (p_Player.isSeleted == true)
    {
        showHints(p_Player);
    }
    else
    {
        resetBoardColor();
    }
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
