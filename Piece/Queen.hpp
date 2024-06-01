#pragma once

#include "Base/Piece.hpp"
#include "../Assets/Container.hpp"

class Queen : public Piece {
public:

    Queen(PieceColor color, int x, int y) : Piece(color, PieceType::QUEEN, x, y) {
        this->buildValidMoves();
    };

protected:
    void buildValidMoves() final {
        Bitboard occupied = Container::getGameState()->calcOccupied();

        for (int i = 0; i < 64; i++) {
            Bitboard moves = 0ULL;

            // Ruchy jak wieża
            for (int j = i + 1; j % 8 != 0; j++) {
                moves |= 1ULL << j;
                if (occupied & (1ULL << j)) break;
            }
            for (int j = i - 1; j % 8 != 7 && j >= 0; j--) {
                moves |= 1ULL << j;
                if (occupied & (1ULL << j)) break;
            }
            for (int j = i + 8; j < 64; j += 8) {
                moves |= 1ULL << j;
                if (occupied & (1ULL << j)) break;
            }
            for (int j = i - 8; j >= 0; j -= 8) {
                moves |= 1ULL << j;
                if (occupied & (1ULL << j)) break;
            }

            // Ruchy jak goniec
            for (int j = i + 9; j % 8 != 0 && j < 64; j += 9) {
                moves |= 1ULL << j;
                if (occupied & (1ULL << j)) break;
            }
            for (int j = i + 7; j % 8 != 7 && j < 64; j += 7) {
                moves |= 1ULL << j;
                if (occupied & (1ULL << j)) break;
            }
            for (int j = i - 7; j % 8 != 0 && j >= 0; j -= 7) {
                moves |= 1ULL << j;
                if (occupied & (1ULL << j)) break;
            }
            for (int j = i - 9; j % 8 != 7 && j >= 0; j -= 9) {
                moves |= 1ULL << j;
                if (occupied & (1ULL << j)) break;
            }

            this->validMoves[i] = moves;
        }
    }
};