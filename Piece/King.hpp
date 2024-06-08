#pragma once

#include "Base/Piece.hpp"

const sf::Color checkColor(237, 78, 92);

class King : public Piece {
public:

    King(PieceColor color, int x, int y, bool lightMode = false) : Piece(color, PieceType::KING, x, y, lightMode) {
        this->checkCircle = new sf::CircleShape(50);
        this->checkCircle->setFillColor(checkColor);
        this->checkCircle->setPosition(
                x * 100,
                y * 100
        );
    };

    static Bitboard getValidMoves(int position, Bitboard occupied) {
        Bitboard bitboard = 1ULL << position;
        Bitboard moves = 0ULL;

        if (position % 8 != 0) {
            moves |= bitboard << 7;
            moves |= bitboard >> 9;
            moves |= bitboard >> 1;
            moves |= bitboard << 9;
        }
        if (position % 8 != 7) {
            moves |= bitboard << 9;
            moves |= bitboard >> 7;
            moves |= bitboard << 1;
            moves |= bitboard >> 9;
        }
        moves |= bitboard << 8;
        moves |= bitboard >> 8;

        return moves;
    }

    ~King() override {
        delete this->checkCircle;
    };

    void draw(sf::RenderTarget& target) {
        if (this->lightMode == true) {
            return;
        }


        if (this->selected) {
            target.draw(*this->background);
        }

        if (this->check) {
            target.draw(*this->checkCircle);
        }

        target.draw(*this->sprite);
    }

public:
    void setCheck(bool check = false) {
        this->check = check;
    }


protected:
    void buildValidMoves(Bitboard captures, Bitboard occupied, MoveHistoryElement* lastMove) final {
        int i = BitBoard::calcShift(this->getPositionX() / 100, this->getPositionY() / 100);

        this->validMoves = King::getValidMoves(i, occupied);
    }

    bool check = false;
    sf::CircleShape* checkCircle = nullptr;
};