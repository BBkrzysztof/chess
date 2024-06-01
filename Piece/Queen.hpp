#pragma once

#include "Base/Piece.hpp"

class Queen : public Piece {
public:

    Queen(PieceColor color, int x, int y) : Piece(color, PieceType::QUEEN, x, y) {};

protected:
    void updateBitBoard(int from) final {

    }

    void buildValidMoves() final {

    }
};