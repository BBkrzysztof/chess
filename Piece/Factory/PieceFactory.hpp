#pragma once

#include "../Base/Piece.hpp"

#include "../Pawn.hpp"
#include "../Bishop.hpp"
#include "../Knight.hpp"
#include "../Rook.hpp"
#include "../Queen.hpp"
#include "../King.hpp"

class PieceFactory {
public:
    static Piece* create(PieceType type, PieceColor color, int x, int y) {

        switch (type) {
            case PieceType::PAWN:
                return new Pawn(color, x, y);
                break;
            case PieceType::KNIGHT:
                return new Knight(color, x, y);
                break;
            case PieceType::BISHOP:
                return new Bishop(color, x, y);
                break;
            case PieceType::ROOK:
                return new Rook(color, x, y);
                break;
            case PieceType::QUEEN:
                return new Queen(color, x, y);
                break;
            case PieceType::KING:
                return new King(color, x, y);
                break;
        }

        return new Pawn(color, x, y);
    }

    static Piece* copy(const Piece* piece) {

        Piece* copy = nullptr;
        PieceColor color = piece->getPieceColor();
        int x = piece->getPositionX() / 100;
        int y = piece->getPositionY() / 100;

        switch (piece->getPieceType()) {
            case PieceType::PAWN:
                copy = new Pawn(color, x, y, true);
                break;
            case PieceType::KNIGHT:
                copy = new Knight(color, x, y, true);
                break;
            case PieceType::BISHOP:
                copy = new Bishop(color, x, y, true);
                break;
            case PieceType::ROOK:
                copy = new Rook(color, x, y, true);
                break;
            case PieceType::QUEEN:
                copy = new Queen(color, x, y, true);
                break;
            case PieceType::KING:
                copy = new King(color, x, y, true);
                break;
        }
        copy->setHash(piece->getHash());


        return copy;
    }


};