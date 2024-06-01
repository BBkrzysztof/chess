#pragma once

#include "Base/Piece.hpp"

class Pawn : public Piece {
public:

    Pawn(PieceColor color, int x, int y) : Piece(color, PieceType::PAWN, x, y) {};

protected:
    void updateBitBoard(int from) final {

    }

    void buildValidMoves() final {

    }
};