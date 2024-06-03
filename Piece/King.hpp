#pragma once

#include "Base/Piece.hpp"
#include "../Assets/Container.hpp"

class King : public Piece {
public:

    King(PieceColor color, int x, int y) : Piece(color, PieceType::KING, x, y) {
        this->buildValidMoves();
    };

    static Bitboard getValidMoves(int position, Bitboard occupied) {
        Bitboard bitboard = 1ULL << position;
        Bitboard moves = 0ULL;

        if (position % 8 != 0) {
            moves |= bitboard << 7;
            moves |= bitboard >> 9;
            moves |= bitboard >> 1;
            moves |= bitboard << 9;
        }
        if (position % 8 != 7) {
            moves |= bitboard << 9;
            moves |= bitboard >> 7;
            moves |= bitboard << 1;
            moves |= bitboard >> 9;
        }
        moves |= bitboard << 8;
        moves |= bitboard >> 8;

        return moves;
    }


protected:
    void buildValidMoves() final {
        Bitboard occupied = Container::getGameState()->calcOccupied();

        int i = BitBoard::calcShift(this->getPositionX() / 100, this->getPositionY() / 100);

        this->validMoves = King::getValidMoves(i, occupied);
    }


};