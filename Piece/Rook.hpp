#pragma once

#include "Base/Piece.hpp"
#include "../Assets/Container.hpp"

class Rook : public Piece {
public:

    Rook(PieceColor color, int x, int y) : Piece(color, PieceType::ROOK, x, y) {
        this->buildValidMoves();
    };

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

protected:

    void buildValidMoves() final {
        Bitboard occupied = Container::getGameState()->calcOccupied();
        int i = BitBoard::calcShift(this->getPositionX() / 100, this->getPositionY() / 100);

        this->validMoves = Rook::getValidMoves(i, occupied);
    }
};