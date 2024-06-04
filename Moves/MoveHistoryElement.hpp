#pragma once

#include "../Piece/Base/Piece.hpp"
#include "../Piece/Base/MoveIndicator.hpp"

class Piece;

class MoveHistoryElement {
public:
    int from;
    int to;
    MoveOptions moveOptions;
    PieceColor color;
    PieceType type;
    Piece* piece;

    MoveHistoryElement() = default;

    MoveHistoryElement(
            int from,
            int to,
            MoveOptions options,
            PieceColor color,
            PieceType type,
            Piece* piece
    ) : from(from), to(to), moveOptions(options), color(color), type(type), piece(piece) {};
};
