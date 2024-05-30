#pragma once

#include "Base/Piece.hpp"

class Pawn : public Piece {
public:

    Pawn(PieceColor color, int x, int y) : Piece(color, PieceType::PAWN, x, y) {};

protected:
    bool validateMove(int x, int y) final {
        return x == 500 && y == 500;
    }

    Bitboard getMoveMask(int x, int y) final{

    }
};