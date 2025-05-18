#include "piecesmanager.h"
#include "piecesdefs.h"
#include <iostream>

void PiecesManager::init()
{
    turn = 1;
    m_IsLastBlackMove = false;
    m_LastPiece_Col = -1;
    m_LastPiece_Row = -1;
    m_PieceSelectState = false;
    m_BoardPieces.fill({PIECES_TYPE::EMPTY});
    m_DrawPieces = new Piece*[8];
    for (int i = 0; i < 8; ++i) {
        m_DrawPieces[i] = new Piece[8];
    }
}

void PiecesManager::initDefaultBoard()
{
    m_BoardPieces = {{
        {{ -5, -4, -3, -2, -1, -3, -4, -5 }},
        {{ -6, -6, -6, -6, -6, -6, -6, -6 }},
        {{  0,  0,  0,  0,  0,  0,  0,  0 }},
        {{  0,  0,  0,  0,  0,  0,  0,  0 }},
        {{  0,  0,  0,  0,  0,  0,  0,  0 }},
        {{  0,  0,  0,  0,  0,  0,  0,  0 }},
        {{  6,  6,  6,  6,  6,  6,  6,  6 }},
        {{  5,  4,  3,  2,  1,  3,  4,  5 }}
    }};
    whiteKingX = 7;
    whiteKingY = 4;
    blackKingX = 0;
    blackKingY = 4;
}

bool PiecesManager::isBlack()
{
    return (m_BoardPieces[m_LastPiece_Row][m_LastPiece_Col] < 0);
}

PiecesManager::PiecesManager(SDL_Renderer* p_Renderer,int BoardSize)
    : m_Renderer(p_Renderer), m_BoardPieceSize(BoardSize/8)
{
    Piece::s_Renderer = m_Renderer;
    init();
    initDefaultBoard();
    CalculatePieces();
    for (int row = 0 ; row <= 7 ; row++)
        for (int col =0 ; col <= 7 ; col++)
            m_DrawPieces[row][col].setPieceSize(m_BoardPieceSize);
}

void PiecesManager::addPiece(const char* p_FilePath, int row, int col)
{
    m_DrawPieces[row][col].setTextureFromPath(p_FilePath);
    m_DrawPieces[row][col].setPosition((col * m_BoardPieceSize), (row * m_BoardPieceSize));
}

void PiecesManager::CalculatePieces()
{
    for (int row = 0 ; row <= 7 ; row++)
    {
        for (int col = 0 ; col <= 7 ; col++)
        {
            switch(m_BoardPieces[row][col])
            {
                case PIECES_TYPE::WHITE_KNIGHT:
                    addPiece(WHITE_KNIGHT_PATH,row,col);
                    break;
                case PIECES_TYPE::BLACK_PAWN:
                    addPiece(BLACK_PAWN_PATH,row,col);
                    break;
                case PIECES_TYPE::WHITE_PAWN:
                    addPiece(WHITE_PAWN_PATH,row,col);
                    break;
                case PIECES_TYPE::WHITE_ROOK:
                    addPiece(WHITE_ROOK_PATH,row,col);
                    break;
                case PIECES_TYPE::BLACK_ROOK:
                    addPiece(BLACK_ROOK_PATH,row,col);
                    break;
                case PIECES_TYPE::BLACK_BISHOP:
                    addPiece(BLACK_BISHOP_PATH,row,col);
                    break;
                case PIECES_TYPE::BLACK_KNIGHT:
                    addPiece(BLACK_KNIGHT_PATH,row,col);
                    break;
                case PIECES_TYPE::BLACK_QUEEN:
                    addPiece(BLACK_QUEEN_PATH,row,col);
                    break;
                case PIECES_TYPE::BLACK_KING:
                    addPiece(BLACK_KING_PATH,row,col);
                    break;
                case PIECES_TYPE::WHITE_KING:
                    addPiece(WHITE_KING_PATH,row,col);
                    break;
                case PIECES_TYPE::WHITE_QUEEN:
                    addPiece(WHITE_QUEEN_PATH,row,col);
                    break;
                case PIECES_TYPE::WHITE_BISHOP:
                    addPiece(WHITE_BISHOP_PATH,row,col);
                    break;
                case PIECES_TYPE::EMPTY:
                    m_DrawPieces[row][col].setTextureFromPath(EMPTY_PATH);
                    m_DrawPieces[row][col].setPosition((col * m_BoardPieceSize), (row * m_BoardPieceSize));
                    break;
                default:
                    break;
            }
        }
    }
}

void PiecesManager::setSize(int p_PieceSize)
{
    m_BoardPieceSize = p_PieceSize;
}
std::string PiecesManager::GameOver()
{
    bool WhiteKing = false;
    bool BlackKing = false;
    for (int row = 0; row <= 7; row++)
    {
        for (int col = 0; col <= 7; col++)
        {
            if (m_BoardPieces[row][col] == PIECES_TYPE::WHITE_KING)
            {
                WhiteKing = true;
            }
            if (m_BoardPieces[row][col] == PIECES_TYPE::BLACK_KING)
            {
                BlackKing = true;
            }
        }
    }
    if (!WhiteKing)
    {
        return "Black";
    }
    if (!BlackKing)
    {
        return "White";
    }
    return "";
}

bool PiecesManager::movePiece(int fromRow, int fromCol, int toRow, int toCol)
{
    if (fromRow < 0 || fromRow >= 8 || fromCol < 0 || fromCol >= 8)
        return false;
    if (toRow < 0 || toRow >= 8 || toCol < 0 || toCol >= 8)
        return false;
    if (m_BoardPieces[fromRow][fromCol] == PIECES_TYPE::EMPTY)
        return false;

    int pieceMoved = m_BoardPieces[fromRow][fromCol];
    int pieceCaptured = m_BoardPieces[toRow][toCol];
    bool isWhite = pieceMoved > 0;

    if ((turn == 1 && pieceMoved < 0) || (turn == -1 && pieceMoved > 0))
        return false;

    if (pieceMoved == PIECES_TYPE::WHITE_KING || pieceMoved == PIECES_TYPE::BLACK_KING)
    {
        if (abs(toCol - fromCol) == 2)
        {
            if (Move::canCastle(m_BoardPieces, isWhite, moveManager.getMoveHistory()))
            {
                m_BoardPieces[toRow][toCol] = pieceMoved;
                m_BoardPieces[fromRow][fromCol] = PIECES_TYPE::EMPTY;

                if (toCol > fromCol)
                {
                    m_BoardPieces[toRow][toCol - 1] = m_BoardPieces[toRow][toCol + 1];
                    m_BoardPieces[toRow][toCol + 1] = PIECES_TYPE::EMPTY;
                }
                else
                {
                    m_BoardPieces[toRow][toCol + 1] = m_BoardPieces[toRow][toCol - 2];
                    m_BoardPieces[toRow][toCol - 2] = PIECES_TYPE::EMPTY;
                }
                moveManager.addMove(fromRow, fromCol, toRow, toCol, pieceMoved, PIECES_TYPE::EMPTY);
                turn = -turn;
                CalculatePieces();
                return true;
            }
            else
            {
                return false;
            }
        }
    }
    if (pieceMoved == PIECES_TYPE::WHITE_PAWN && toRow == 0)
    {
        PiecesManager::promotePawn(toRow, toCol, true);
        m_BoardPieces[fromRow][fromCol] = PIECES_TYPE::EMPTY;
        moveManager.addMove(fromRow, fromCol, toRow, toCol, pieceMoved, PIECES_TYPE::EMPTY);
        turn = -turn;
        CalculatePieces();
        return true;
    }
    else if (pieceMoved == PIECES_TYPE::BLACK_PAWN && toRow == 7)
    {
        PiecesManager::promotePawn(toRow, toCol, false);
        m_BoardPieces[fromRow][fromCol] = PIECES_TYPE::EMPTY;
        moveManager.addMove(fromRow, fromCol, toRow, toCol, pieceMoved, PIECES_TYPE::EMPTY);
        turn = -turn;
        CalculatePieces();
        return true;
    }

    if (!Move::ValidMove(m_BoardPieces, PiecePosition(fromRow, fromCol), PiecePosition(toRow, toCol), moveManager.getMoveHistory()))
        return false;

    m_BoardPieces[toRow][toCol] = pieceMoved;
    m_BoardPieces[fromRow][fromCol] = PIECES_TYPE::EMPTY;

    if (pieceMoved == PIECES_TYPE::WHITE_KING)
    {
        whiteKingX = toRow;
        whiteKingY = toCol;
    }
    else if (pieceMoved == PIECES_TYPE::BLACK_KING)
    {
        blackKingX = toRow;
        blackKingY = toCol;
    }

    moveManager.addMove(fromRow, fromCol, toRow, toCol, pieceMoved, pieceCaptured);
    turn = -turn;

    m_LastPiece_Row = toRow;
    m_LastPiece_Col = toCol;
    m_IsLastBlackMove = isBlack();

    CalculatePieces();
    return true;
}
void PiecesManager::promotePawn(int row, int col, bool isWhite)
{
    std::cout << "PROMOTION!!!" << std::endl;
    int choice = showPromotionMenu(isWhite);
    switch (choice)
    {
        case 0:
            m_BoardPieces[row][col] = isWhite ? PIECES_TYPE::WHITE_QUEEN : PIECES_TYPE::BLACK_QUEEN;
            break;
        case 1:
            m_BoardPieces[row][col] = isWhite ? PIECES_TYPE::WHITE_ROOK : PIECES_TYPE::BLACK_ROOK;
            break;
        case 2:
            m_BoardPieces[row][col] = isWhite ? PIECES_TYPE::WHITE_BISHOP : PIECES_TYPE::BLACK_BISHOP;
            break;
        case 3:
            m_BoardPieces[row][col] = isWhite ? PIECES_TYPE::WHITE_KNIGHT : PIECES_TYPE::BLACK_KNIGHT;
            break;
    }
    std::cout << "PROMOTION COMPLETED!!!" << std::endl;
}
int PiecesManager::showPromotionMenu(bool isWhite)
{
    std::string color = isWhite ? "White" : "Black";
    std::cout << color << " Pawn Promotion!\n";
    std::cout << "Choose piece to promote to:\n";
    std::cout << "0: Queen\n1: Rook\n2: Bishop\n3: Knight\n";
    int choice;
    std::cin >> choice;
    return choice;
}
void PiecesManager::resetPieces()
{
    initDefaultBoard();
    CalculatePieces();
}
Player PiecesManager::getPlayer() const
{
    Player _player(m_BoardPieces);
    _player.isSeleted = m_PieceSelectState;
    _player.row = m_LastPiece_Row;
    _player.col = m_LastPiece_Col;

    return _player;
}

void PiecesManager::drawPieces()
{
    for (int row = 0; row <= 7 ;row++)
    {
        for (int col = 0 ; col <= 7; col++)
        {
            m_DrawPieces[row][col].draw();
        }
    }
}

void PiecesManager::updateBoardPieces(int* p_MouseX,int* p_MouseY)
{
    int _X = *p_MouseX - m_BoardPieceSize / 2;
    int _Y = *p_MouseY - m_BoardPieceSize / 2;
    if (m_PieceSelectState)
    {
        m_DrawPieces[m_LastPiece_Row][m_LastPiece_Col].setPosition(_X ,_Y );
    }
}

void PiecesManager::isPieceSelect(bool p_state, int* p_MouseX, int* p_MouseY)
{
    if (p_state)
    {
        m_PieceSelectState = true;
        m_LastPiece_Col = *p_MouseX / m_BoardPieceSize;
        m_LastPiece_Row = *p_MouseY / m_BoardPieceSize;
    }
    else
    {
        m_PieceSelectState = false;
        int newCol = *p_MouseX / m_BoardPieceSize;
        int newRow = *p_MouseY / m_BoardPieceSize;

        movePiece(m_LastPiece_Row, m_LastPiece_Col, newRow, newCol);

        CalculatePieces();
        GameOver();
    }
}



