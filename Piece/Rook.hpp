#pragma once

#include "Base/Piece.hpp"

class Rook : public Piece {
public:

    Rook(PieceColor color, int x, int y) : Piece(color, PieceType::ROOK, x, y) {};

protected:
    void updateBitBoard(int from) final {

    }

    void buildValidMoves() final{

    }
};