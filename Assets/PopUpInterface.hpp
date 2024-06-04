#pragma once

#include "../Piece/Base/Piece.hpp"

class PopUpInterface {
public:
    virtual PieceType draw() = 0;
};