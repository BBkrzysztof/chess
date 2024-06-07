#pragma once

#include "Base/Piece.hpp"

class Rook : public Piece {
public:

    Rook(PieceColor color, int x, int y, bool lightMode = false) : Piece(color, PieceType::ROOK, x, y, lightMode) {};

    static Bitboard getValidMoves(int position, Bitboard occupied) {
        Bitboard moves = 0ULL;

        // Przesuwanie w prawo
        for (int j = position + 1; j % 8 != 0; j++) {
            moves |= 1ULL << j;
            if (occupied & (1ULL << j)) break; // Zatrzymanie, jeśli pole jest zajęte
        }
        // Przesuwanie w lewo
        for (int j = position - 1; j % 8 != 7 && j >= 0; j--) {
            moves |= 1ULL << j;
            if (occupied & (1ULL << j)) break;
        }
        // Przesuwanie w górę
        for (int j = position + 8; j < 64; j += 8) {
            moves |= 1ULL << j;
            if (occupied & (1ULL << j)) break;
        }
        // Przesuwanie w dół
        for (int j = position - 8; j >= 0; j -= 8) {
            moves |= 1ULL << j;
            if (occupied & (1ULL << j)) break;
        }

        return moves;
    }

    ~Rook() override = default;

protected:

    void buildValidMoves(Bitboard captures, Bitboard occupied, MoveHistoryElement* lastMove) final {
        int i = BitBoard::calcShift(this->getPositionX() / 100, this->getPositionY() / 100);

        this->validMoves = Rook::getValidMoves(i, occupied);
    }
};