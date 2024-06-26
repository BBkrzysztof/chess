#pragma once

#include "Base/Piece.hpp"

class Bishop : public Piece {
public:

    Bishop(PieceColor color, int x, int y, bool lightMode = false) : Piece(color, PieceType::BISHOP, x, y,
                                                                           lightMode) {};

    static Bitboard getValidMoves(int position, Bitboard occupied) {
        Bitboard moves = 0ULL;

        // Przesuwanie w prawo w górę
        for (int j = position + 9; j % 8 != 0 && j < 64; j += 9) {
            moves |= 1ULL << j;
            if (occupied & (1ULL << j)) break;
        }
        // Przesuwanie w lewo w górę
        for (int j = position + 7; j % 8 != 7 && j < 64; j += 7) {
            moves |= 1ULL << j;
            if (occupied & (1ULL << j)) break;
        }
        // Przesuwanie w prawo w dół
        for (int j = position - 7; j % 8 != 0 && j >= 0; j -= 7) {
            moves |= 1ULL << j;
            if (occupied & (1ULL << j)) break;
        }
        // Przesuwanie w lewo w dół
        for (int j = position - 9; j % 8 != 7 && j >= 0; j -= 9) {
            moves |= 1ULL << j;
            if (occupied & (1ULL << j)) break;
        }

        return moves;
    }

    ~Bishop() override = default;

protected:
    void buildValidMoves(Bitboard captures, Bitboard occupied, MoveHistoryElement* lastMove) final {

        int i = BitBoard::calcShift(this->getPositionX() / 100, this->getPositionY() / 100);

        this->validMoves = Bishop::getValidMoves(i, occupied);
    }
};