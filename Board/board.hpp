#pragma once

#include <map>
#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>

#include "../Piece/King.hpp"
#include "../Piece/Base/Piece.hpp"


const sf::Color lightColor(240, 217, 181);
const sf::Color darkColor(181, 136, 99);
const int tileSize = 100;

class Board {
public:
    Board() {
        auto* k1 = new King(PieceColor::BLACK_PIECE, 0, 0);
        auto* k2 = new King(PieceColor::WHITE_PIECE, 700, 700);

        this->registerPiece(k1);
        this->registerPiece(k2);
    };

    void draw(sf::RenderTarget& target) {
        // draw board
        for (int row = 0; row < 8; ++row) {
            for (int col = 0; col < 8; ++col) {
                sf::RectangleShape tile(sf::Vector2f(tileSize, tileSize));
                tile.setPosition(col * tileSize, row * tileSize);

                if ((row + col) % 2 == 0) {
                    tile.setFillColor(lightColor);
                } else {
                    tile.setFillColor(darkColor);
                }

                target.draw(tile);
            }
        }

        // draw pieces

        for (const auto& element: this->pieces) {

            if (element.first == this->selectedPiece) {
                element.second->selectPiece();
            } else {
                element.second->unSelectPiece();
            }

            element.second->draw(target);
        }
    }

    static std::string pieceKey(Piece* piece) {
        std::stringstream s;
        s << piece->getPieceType() << "-" << piece->getPieceColor();
        return s.str();
    }

    std::map<std::string, Piece*> getPieces() {
        return this->pieces;
    }

    void setSelectedPiece(std::string pieceHash) {
        this->selectedPiece = pieceHash;
    }

    std::string getSelectedPiece() {
        return this->selectedPiece;
    }

private:

    void registerPiece(Piece* piece) {
        this->pieces[Board::pieceKey(piece)] = piece;
    }


private:
    std::map<std::string, Piece*> pieces;
    std::string selectedPiece;
    bool isCheck;
    bool isCheckmate;
    bool isStalemate;

};