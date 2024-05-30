#pragma once

#include "Base/Piece.hpp"

class King : public Piece {
public:

    King(PieceColor color, int x, int y) : Piece(color, PieceType::KING, x, y) {};

protected:
    bool validateMove(int x, int y) final{
        return true;
    }
};