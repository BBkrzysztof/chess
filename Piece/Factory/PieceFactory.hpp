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


};