#pragma once

#include "Base/Piece.hpp"

class Knight : public Piece {
public:

    Knight(PieceColor color, int x, int y) : Piece(color, PieceType::KNIGHT, x, y) {};

    static Bitboard getValidMoves(int position, Bitboard occupied) {
        Bitboard bitboard = 1ULL << position;
        Bitboard moves = 0ULL;

        if (position % 8 > 1 && position / 8 < 7) moves |= bitboard << 6;
        if (position % 8 < 6 && position / 8 < 7) moves |= bitboard << 10;
        if (position % 8 < 7 && position / 8 < 6) moves |= bitboard << 17;
        if (position % 8 > 0 && position / 8 < 6) moves |= bitboard << 15;
        if (position % 8 > 1 && position / 8 > 0) moves |= bitboard >> 10;
        if (position % 8 < 6 && position / 8 > 0) moves |= bitboard >> 6;
        if (position % 8 < 7 && position / 8 > 1) moves |= bitboard >> 15;
        if (position % 8 > 0 && position / 8 > 1) moves |= bitboard >> 17;

        return moves;
    }

protected:

    void buildValidMoves(Bitboard captures, Bitboard occupied, MoveHistoryElement* lastMove) final {
        int i = BitBoard::calcShift(this->getPositionX() / 100, this->getPositionY() / 100);

        this->validMoves = Knight::getValidMoves(i, occupied);
    }
};