#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <iostream>

const std::string PATH = "../Piece/icons/";

sf::Color bgColor(255, 240, 170);
typedef uint64_t Bitboard;

enum PieceColor {
    WHITE_PIECE = 1,
    BLACK_PIECE = 2
};

enum PieceType {
    PAWN = 0,
    KNIGHT = 1,
    BISHOP = 2,
    ROOK = 3,
    QUEEN = 4,
    KING = 5,
};

const std::map<PieceType, std::string> icons = {
        {PieceType::PAWN,   "P.png"},
        {PieceType::KNIGHT, "N.png"},
        {PieceType::BISHOP, "B.png"},
        {PieceType::ROOK,   "R.png"},
        {PieceType::QUEEN,  "Q.png"},
        {PieceType::KING,   "K.png"},
};

class Piece {
public:

    Piece(PieceColor color, PieceType type, int x, int y) {

        this->positionX = x * 100;
        this->positionY = y * 100;

        this->pieceType = type;
        this->pieceColor = color;
        std::cout << "Loaded texture: " << this->buildIconPath() << std::endl;

        if (!this->texture.loadFromFile(this->buildIconPath())) {
            throw std::exception();
        }

        this->background.setSize(sf::Vector2f(100, 100));
        this->background.setFillColor(bgColor);

        this->circleShape.setRadius(50);
        this->circleShape.setFillColor(bgColor);

        this->sprite.setTexture(this->texture);
        this->resetPosition();
    }

    void draw(sf::RenderTarget& target) {
        if (this->selected) {
            target.draw(this->background);
        }

        if (this->beatable) {
            target.draw(this->circleShape);
        }

        target.draw(this->sprite);
    }

    void move(int x, int y) {
        this->positionX = x * 100;
        this->positionY = y * 100;
        this->resetPosition();
    }

    bool validateBounds(const sf::Vector2i& mousePosition) {
        sf::FloatRect bounds = this->sprite.getGlobalBounds();
        return bounds.contains(static_cast<sf::Vector2f>(mousePosition));
    }

    void selectPiece() {
        this->selected = true;
    }

    void unSelectPiece() {
        this->selected = false;
    }

    void selectBeatable() {
        this->beatable = true;
    }

    void unSelectBeatable() {
        this->beatable = false;
    }

    PieceColor getPieceColor() const {
        return this->pieceColor;
    }

    PieceType getPieceType() const {
        return this->pieceType;
    }

    int getPositionX() const {
        return this->positionX;
    }

    int getPositionY() const {
        return this->positionY;
    }

    Bitboard getValidMoves() {
        int row = (this->positionY / 100) * 8;
        int shift = row + (this->positionX / 100);
        return this->validMoves[shift];
    }

protected:
    virtual void updateBitBoard(int from) = 0;

    virtual void buildValidMoves() = 0;

private:

    void resetPosition() {
        this->sprite.setPosition(
                this->positionX,
                this->positionY
        );

        this->background.setPosition(
                this->positionX,
                this->positionY
        );

        this->circleShape.setPosition(
                this->positionX,
                this->positionY
        );
    }

    std::string buildIconPath() {
        return PATH + this->buildIconColor(icons.at(this->pieceType));
    }

    std::string buildIconColor(std::string icon) {
        if (this->pieceColor == PieceColor::BLACK_PIECE) {
            return "_" + icon;
        }
        return icon;
    }

protected:
    Bitboard validMoves[64];

private:
    int positionX = 0;
    int positionY = 0;

    bool selected = false;
    bool beatable = false;

    PieceColor pieceColor;
    PieceType pieceType;

    sf::Sprite sprite;
    sf::Texture texture;
    sf::RectangleShape background;
    sf::CircleShape circleShape;
};