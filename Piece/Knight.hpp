#pragma once

#include "Base/Piece.hpp"

class Knight : public Piece {
public:

    Knight(PieceColor color, int x, int y) : Piece(color, PieceType::KNIGHT, x, y) {};

protected:
    bool validateMove(int x, int y) final{
        return true;
    }

    Bitboard getMoveMask(int x, int y) final{

    }
};