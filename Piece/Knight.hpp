#pragma once

#include "Base/Piece.hpp"

class Knight : public Piece {
public:

    Knight(PieceColor color, int x, int y) : Piece(color, PieceType::KNIGHT, x, y) {
        this->buildValidMoves();
    };

protected:

    void buildValidMoves() final {
        int i = BitBoard::calcShift(this->getPositionX() / 100, this->getPositionY() / 100);

        Bitboard bitboard = 1ULL << i;
        Bitboard moves = 0ULL;

        if (i % 8 > 1 && i / 8 < 7) moves |= bitboard << 6;
        if (i % 8 < 6 && i / 8 < 7) moves |= bitboard << 10;
        if (i % 8 < 7 && i / 8 < 6) moves |= bitboard << 17;
        if (i % 8 > 0 && i / 8 < 6) moves |= bitboard << 15;
        if (i % 8 > 1 && i / 8 > 0) moves |= bitboard >> 10;
        if (i % 8 < 6 && i / 8 > 0) moves |= bitboard >> 6;
        if (i % 8 < 7 && i / 8 > 1) moves |= bitboard >> 15;
        if (i % 8 > 0 && i / 8 > 1) moves |= bitboard >> 17;
        this->validMoves = moves;

    }
};