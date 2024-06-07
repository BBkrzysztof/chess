#pragma once

#include <cstdlib>
#include <functional>

#include "Base/Piece.hpp"

class Pawn : public Piece {
public:

    Pawn(PieceColor color, int x, int y, bool lightMode = false) : Piece(color, PieceType::PAWN, x, y, lightMode) {
    };

    static Bitboard getValidMoves(
            int position,
            Bitboard occupied,
            Bitboard captures,
            PieceColor color,
            std::function<void(Bitboard current)> callback
    ) {
        if (color == PieceColor::BLACK_PIECE) {
            Bitboard bitboard = 1ULL << position;
            Bitboard moves = 0ULL;

            // Przesuwanie o jedno pole w górę
            if (position / 8 != 7) {
                if (!(captures & (bitboard << 8))) {
                    moves |= bitboard << 8;
                }
            }
            // Bicie w lewo i prawo
            if (position % 8 != 0 && position / 8 != 7) {
                callback(bitboard << 7);

                if (captures & (bitboard << 7)) {
                    moves |= bitboard << 7;
                }
            }
            if (position % 8 != 7 && position / 8 != 7) {
                callback(bitboard << 9);

                if (captures & (bitboard << 9)) {
                    moves |= bitboard << 9;
                }
            }
            // Przesuwanie o dwa pola z pozycji startowej
            if (position / 8 == 1) {
                if (!(captures & (bitboard << 16))) {
                    if (!(occupied & (bitboard << 8))) {
                        moves |= bitboard << 16;
                    }
                }
            }

            return moves;

        } else {

            Bitboard bitboard = 1ULL << position;
            Bitboard moves = 0ULL;

            // Przesuwanie o jedno pole w dół
            if (position / 8 != 0) {
                if (!(captures & (bitboard >> 8))) {
                    moves |= bitboard >> 8;
                }
            }
            // Bicie w lewo i prawo
            if (position % 8 != 0 && position / 8 != 0) {
                callback(bitboard >> 9);

                if (captures & (bitboard >> 9)) {
                    moves |= bitboard >> 9;
                }
            }
            if (position % 8 != 7 && position / 8 != 0) {
                callback(bitboard >> 7);

                if (captures & (bitboard >> 7)) {
                    moves |= bitboard >> 7;
                }
            }
            // Przesuwanie o dwa pola z pozycji startowej
            if (position / 8 == 6) {
                if (!(captures & (bitboard >> 16))) {
                    if (!(occupied & (bitboard >> 8))) {
                        moves |= bitboard >> 16;
                    }
                }
            }

            return moves;
        }
    }

    ~Pawn() override = default;

protected:

    void buildValidMoves(Bitboard captures, Bitboard occupied, MoveHistoryElement* lastMove) final {

        int i = BitBoard::calcShift(this->getPositionX() / 100, this->getPositionY() / 100);

        this->validMoves = Pawn::getValidMoves(i, occupied, captures, this->getPieceColor(), [](Bitboard board) {});
    }
};