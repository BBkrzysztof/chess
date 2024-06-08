#pragma once

#include <SFML/Graphics.hpp>
#include <map>
#include <iostream>

#include "../../Assets/BitBoard.hpp"
#include "../../Moves/MoveHistoryElement.hpp"

const std::string PATH = "../Piece/icons/";

sf::Color bgColor(255, 240, 170);
typedef uint64_t Bitboard;

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

    Piece(PieceColor color, PieceType type, int x, int y, bool lightMode = false) {

        this->positionX = x * 100;
        this->positionY = y * 100;

        this->pieceType = type;
        this->pieceColor = color;

        this->lightMode = lightMode;
        if (this->lightMode == false) {
            this->texture = new sf::Texture();
            this->background = new sf::RectangleShape();
            this->circleShape = new sf::CircleShape();
            this->sprite = new sf::Sprite();

            if (!this->texture->loadFromFile(this->buildIconPath())) {
                throw std::exception();
            }

            this->background->setSize(sf::Vector2f(100, 100));
            this->background->setFillColor(bgColor);

            this->sprite->setTexture(*this->texture);
            this->resetPosition();
        }
    }

    virtual ~Piece() {
        delete this->sprite;
        delete this->texture;
        delete this->background;
        delete this->circleShape;
    }

    void move(int x, int y) {
        this->positionX = x * 100;
        this->positionY = y * 100;
        this->resetPosition();
    }

    bool validateBounds(const sf::Vector2i& mousePosition) {
        sf::FloatRect bounds = this->sprite->getGlobalBounds();
        return bounds.contains(static_cast<sf::Vector2f>(mousePosition));
    }

    void selectPiece() {
        this->selected = true;
    }

    void unSelectPiece() {
        this->selected = false;
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
        return this->validMoves;
    }

    bool operator==(const Piece& other) {
        return this->hash == other.hash;
    }

    void setHash(const std::string& hash) {
        this->hash = hash;
    }

    std::string getHash() const {
        return this->hash;
    }

    void rebuildValidMoves(Bitboard captures, Bitboard occupied, MoveHistoryElement* lastMove) {
        this->buildValidMoves(captures, occupied, lastMove);
    }

    virtual void draw(sf::RenderTarget& target) {
        if (this->lightMode == true) {
            return;
        }

        if (this->selected) {
            target.draw(*this->background);
        }

        target.draw(*this->sprite);
    }

protected:

    virtual void buildValidMoves(Bitboard captures, Bitboard occupied, MoveHistoryElement* lastMove) = 0;

    bool lightMode;
    bool selected = false;
    sf::RectangleShape* background = nullptr;
    sf::Sprite* sprite = nullptr;
private:

    void resetPosition() {

        if (this->lightMode == true) {
            return;
        }

        this->sprite->setPosition(
                this->positionX,
                this->positionY
        );

        this->background->setPosition(
                this->positionX,
                this->positionY
        );

        this->circleShape->setPosition(
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
    Bitboard validMoves;

private:
    int positionX = 0;
    int positionY = 0;

    PieceColor pieceColor;
    PieceType pieceType;

    std::string hash;

    sf::Texture* texture = nullptr;
    sf::CircleShape* circleShape = nullptr;
};
