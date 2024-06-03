#pragma once

#include "Base/Piece.hpp"
#include "../Assets/Container.hpp"

class Queen : public Piece {
public:

    Queen(PieceColor color, int x, int y) : Piece(color, PieceType::QUEEN, x, y) {
        this->buildValidMoves();
    };

    static Bitboard getValidMoves(int position, Bitboard occupied) {
        Bitboard moves = 0ULL;

        // Ruchy jak wieża
        for (int j = position + 1; j % 8 != 0; j++) {
            moves |= 1ULL << j;
            if (occupied & (1ULL << j)) break;
        }
        for (int j = position - 1; j % 8 != 7 && j >= 0; j--) {
            moves |= 1ULL << j;
            if (occupied & (1ULL << j)) break;
        }
        for (int j = position + 8; j < 64; j += 8) {
            moves |= 1ULL << j;
            if (occupied & (1ULL << j)) break;
        }
        for (int j = position - 8; j >= 0; j -= 8) {
            moves |= 1ULL << j;
            if (occupied & (1ULL << j)) break;
        }

        // Ruchy jak goniec
        for (int j = position + 9; j % 8 != 0 && j < 64; j += 9) {
            moves |= 1ULL << j;
            if (occupied & (1ULL << j)) break;
        }
        for (int j = position + 7; j % 8 != 7 && j < 64; j += 7) {
            moves |= 1ULL << j;
            if (occupied & (1ULL << j)) break;
        }
        for (int j = position - 7; j % 8 != 0 && j >= 0; j -= 7) {
            moves |= 1ULL << j;
            if (occupied & (1ULL << j)) break;
        }
        for (int j = position - 9; j % 8 != 7 && j >= 0; j -= 9) {
            moves |= 1ULL << j;
            if (occupied & (1ULL << j)) break;
        }

        return moves;
    }

protected:
    void buildValidMoves() final {
        Bitboard occupied = Container::getGameState()->calcOccupied();
        int i = BitBoard::calcShift(this->getPositionX() / 100, this->getPositionY() / 100);

        this->validMoves = Queen::getValidMoves(i, occupied);
    }
};