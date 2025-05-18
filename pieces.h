#pragma once
#include <SDL.h>
#include "piecesdefs.h"

class PiecePosition {
public:
    PiecePosition() : Row(0), Col(0) {}
	PiecePosition(int row, int col) :Row{row }, Col{ col } {};
	int Row, Col;
	bool operator==(const PiecePosition& other) const {
		return Row == other.Row && Col == other.Col;
	}
};

class Piece
{
public:
    Piece(const char* p_FilePath,int p_PieceSize);
    Piece(){initVariable();}

    ~Piece(){}
    enum PieceColor  { NONE,BLACK,WHITE };
    enum PieceType { PAWN, ROOK, KNIGHT, BISHOP, KING, QUEEN, EMPTY };
    enum MoveType { NORMAL, CASTLE, ENPASSANT, NEWPIECE, INIT };

    void init(const char* p_FilePath,int p_PieceSize);
    void setPosition(int p_X ,int  p_Y);
    void setOrgin(int p_OffsetX , int p_OffsetY);
    void setTextureFromPath(const char* p_FilePath);
    void setPieceSize(int p_Size);
    void Log();
    void draw();

    static SDL_Renderer *s_Renderer;
    static void setRenderer(SDL_Renderer* p_Renderer);
    static SDL_Renderer* GetRenderer();
private:
    SDL_Rect *m_PieceProperty;
    SDL_Texture* m_PieceTexture;
    bool m_CancelDraw;
    int  m_OrginOffsetX;
    int  m_OrginOffsetY;
    void initVariable();
};
