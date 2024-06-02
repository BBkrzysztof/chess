#pragma once

#include <cstdlib>

#include "Base/Piece.hpp"
#include "../Assets/Container.hpp"

class Pawn : public Piece {
public:

    Pawn(PieceColor color, int x, int y) : Piece(color, PieceType::PAWN, x, y) {
        this->buildValidMoves();
    };

protected:

    void buildValidMoves() final {

        Bitboard captures = Container::getGameState()->calcBeatable(this->getPieceColor());
        Bitboard occupied = Container::getGameState()->calcOccupied();
        MoveElement* lastMove = Container::getGameState()->peekHistory();

        int i = BitBoard::calcShift(this->getPositionX() / 100, this->getPositionY() / 100);

        if (this->getPieceColor() == PieceColor::BLACK_PIECE) {
            Bitboard bitboard = 1ULL << i;
            Bitboard moves = 0ULL;

            // Przesuwanie o jedno pole w górę
            if (i / 8 != 7) {
                if (!(captures & (bitboard << 8))) {
                    moves |= bitboard << 8;
                }
            }
            // Bicie w lewo i prawo
            if (i % 8 != 0 && i / 8 != 7) {
                this->isValidEnPassantMove(lastMove, (bitboard << 7));

                if (captures & (bitboard << 7)) {
                    moves |= bitboard << 7;
                }
            }
            if (i % 8 != 7 && i / 8 != 7) {
                this->isValidEnPassantMove(lastMove, (bitboard << 9));

                if (captures & (bitboard << 9)) {
                    moves |= bitboard << 9;
                }
            }
            // Przesuwanie o dwa pola z pozycji startowej
            if (i / 8 == 1) {
                if (!(captures & (bitboard << 16))) {
                    if (!(occupied & (bitboard << 8))) {
                        moves |= bitboard << 16;
                    }
                }
            }

            this->validMoves = moves;

        } else {

            Bitboard bitboard = 1ULL << i;
            Bitboard moves = 0ULL;

            // Przesuwanie o jedno pole w dół
            if (i / 8 != 0) {
                if (!(captures & (bitboard >> 8))) {
                    moves |= bitboard >> 8;
                }
            }
            // Bicie w lewo i prawo
            if (i % 8 != 0 && i / 8 != 0) {
                this->isValidEnPassantMove(lastMove, (bitboard >> 9));

                if (captures & (bitboard >> 9)) {
                    moves |= bitboard >> 9;
                }
            }
            if (i % 8 != 7 && i / 8 != 0) {
                this->isValidEnPassantMove(lastMove, (bitboard >> 7));

                if (captures & (bitboard >> 7)) {
                    moves |= bitboard >> 7;
                }
            }
            // Przesuwanie o dwa pola z pozycji startowej
            if (i / 8 == 6) {
                if (!(captures & (bitboard >> 16))) {
                    if (!(occupied & (bitboard >> 8))) {
                        moves |= bitboard >> 16;
                    }
                }
            }

            this->validMoves = moves;
        }

    }

    void isValidEnPassantMove(MoveElement* lastMove, Bitboard current) {

        if (lastMove == nullptr) {
            return;
        }

        int shift = abs(lastMove->to - lastMove->from);

        if (lastMove->type != PieceType::PAWN || (shift != 16)) {
            return;
        }

        Bitboard pawns = Container::getGameState()->getBitBoard(lastMove->type, lastMove->color);

        Bitboard reverse = BitBoard::moveOnBitBoard(pawns, lastMove->to, lastMove->from);

        if (reverse & current) {
            Container::getGameState()->setEnPassantMove(current);
        } else {
            Container::getGameState()->setEnPassantMove(0LL);
        }
    }
};