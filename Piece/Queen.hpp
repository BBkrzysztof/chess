#pragma once

#include "Base/Piece.hpp"

class Queen : public Piece {
public:

    Queen(PieceColor color, int x, int y) : Piece(color, PieceType::QUEEN, x, y) {};

protected:
    bool validateMove(int x, int y) final{
        return true;
    }

    Bitboard getMoveMask(int x, int y) final{

    }
};