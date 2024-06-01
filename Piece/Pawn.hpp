#pragma once

#include "Base/Piece.hpp"

class Pawn : public Piece {
public:

    Pawn(PieceColor color, int x, int y) : Piece(color, PieceType::PAWN, x, y) {
        this->buildValidMoves();
    };

protected:

    void buildValidMoves() final {
        if (this->getPieceColor() == PieceColor::BLACK_PIECE) {
            for (int i = 0; i < 64; i++) {
                Bitboard bitboard = 1ULL << i;
                Bitboard moves = 0ULL;

                // Przesuwanie o jedno pole w górę
                if (i / 8 != 7) {
                    moves |= bitboard << 8;
                }
                // Bicie w lewo i prawo
                if (i % 8 != 0 && i / 8 != 7) {
                    moves |= bitboard << 7;
                }
                if (i % 8 != 7 && i / 8 != 7) {
                    moves |= bitboard << 9;
                }
                // Przesuwanie o dwa pola z pozycji startowej
                if (i / 8 == 1) {
                    moves |= bitboard << 16;
                }

                this->validMoves[i] = moves;
            }
        } else {
            for (int i = 0; i < 64; i++) {
                Bitboard bitboard = 1ULL << i;
                Bitboard moves = 0ULL;

                // Przesuwanie o jedno pole w dół
                if (i / 8 != 0) {
                    moves |= bitboard >> 8;
                }
                // Bicie w lewo i prawo
                if (i % 8 != 0 && i / 8 != 0) {
                    moves |= bitboard >> 9;
                }
                if (i % 8 != 7 && i / 8 != 0) {
                    moves |= bitboard >> 7;
                }
                // Przesuwanie o dwa pola z pozycji startowej
                if (i / 8 == 6) {
                    moves |= bitboard >> 16;
                }

                this->validMoves[i] = moves;
            }
        }
    }
};