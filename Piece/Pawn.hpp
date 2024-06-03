#pragma once

#include <cstdlib>

#include "Base/Piece.hpp"
#include "../Assets/Container.hpp"

class Pawn : public Piece {
public:

    Pawn(PieceColor color, int x, int y) : Piece(color, PieceType::PAWN, x, y) {
        this->buildValidMoves();
    };

    static void isValidEnPassantMove(MoveElement* lastMove, Bitboard current) {

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

    static Bitboard getValidMoves(
            int position,
            Bitboard occupied,
            Bitboard captures,
            MoveElement* lastMove,
            PieceColor color
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
                Pawn::isValidEnPassantMove(lastMove, (bitboard << 7));

                if (captures & (bitboard << 7)) {
                    moves |= bitboard << 7;
                }
            }
            if (position % 8 != 7 && position / 8 != 7) {
                Pawn::isValidEnPassantMove(lastMove, (bitboard << 9));

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
                Pawn::isValidEnPassantMove(lastMove, (bitboard >> 9));

                if (captures & (bitboard >> 9)) {
                    moves |= bitboard >> 9;
                }
            }
            if (position % 8 != 7 && position / 8 != 0) {
                Pawn::isValidEnPassantMove(lastMove, (bitboard >> 7));

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

protected:

    void buildValidMoves() final {

        Bitboard captures = Container::getGameState()->calcBeatable(this->getPieceColor());
        Bitboard occupied = Container::getGameState()->calcOccupied();
        MoveElement* lastMove = Container::getGameState()->peekHistory();

        int i = BitBoard::calcShift(this->getPositionX() / 100, this->getPositionY() / 100);

        this->validMoves = Pawn::getValidMoves(i, occupied, captures, lastMove, this->getPieceColor());
    }
};