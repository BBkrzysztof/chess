#pragma once

#include "Base/Piece.hpp"

class King : public Piece {
public:

    King(PieceColor color, int x, int y) : Piece(color, PieceType::KING, x, y) {
        this->buildValidMoves();
    };

protected:
    void buildValidMoves() final {
        int i = BitBoard::calcShift(this->getPositionX() / 100, this->getPositionY() / 100);

        Bitboard bitboard = 1ULL << i;
        Bitboard moves = 0ULL;

        if (i % 8 != 0) {
            moves |= bitboard << 7;
            moves |= bitboard >> 9;
            moves |= bitboard >> 1;
            moves |= bitboard << 9;
        }
        if (i % 8 != 7) {
            moves |= bitboard << 9;
            moves |= bitboard >> 7;
            moves |= bitboard << 1;
            moves |= bitboard >> 9;
        }
        moves |= bitboard << 8;
        moves |= bitboard >> 8;

        this->validMoves = moves;

    }
};