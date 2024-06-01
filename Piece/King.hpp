#pragma once

#include "Base/Piece.hpp"

class King : public Piece {
public:

    King(PieceColor color, int x, int y) : Piece(color, PieceType::KING, x, y) {};

protected:
    void updateBitBoard(int from) final{

    }

    void buildValidMoves() final{

    }
};