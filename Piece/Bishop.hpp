#pragma once

#include "Base/Piece.hpp"

class Bishop : public Piece {
public:

    Bishop(PieceColor color, int x, int y) : Piece(color, PieceType::BISHOP, x, y) {};

protected:
    void updateBitBoard(int from) final{

    }

    void buildValidMoves() final{

    }
};