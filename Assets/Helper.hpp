#pragma once

#include <iostream>
#include <sstream>

#include "../Piece/Base/Piece.hpp"

class Helper {
public:
    static std::string getHash(Piece* piece) {

        static int id = 0;
        std::stringstream s;
        s << piece->getPieceType() << "-" << piece->getPieceColor() << "-" << id;
        piece->setHash(s.str());
        id += 1;
        return s.str();
    }
};