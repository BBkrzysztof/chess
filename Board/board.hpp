#pragma once

#include <SFML/Graphics.hpp>
#include "../Piece/King.hpp"
#include "../Piece/Base/Piece.hpp"

const sf::Color lightColor(240, 217, 181);
const sf::Color darkColor(181, 136, 99);
const int tileSize = 100;

class Board {
public:
    Board() = default;

    static void draw(sf::RenderTarget& target) {
        // Rysowanie planszy szachowej
        for (int row = 0; row < 8; ++row) {
            for (int col = 0; col < 8; ++col) {
                sf::RectangleShape tile(sf::Vector2f(tileSize, tileSize));
                tile.setPosition(col * tileSize, row * tileSize);

                // Ustal kolor
                if ((row + col) % 2 == 0) {
                    tile.setFillColor(lightColor);
                } else {
                    tile.setFillColor(darkColor);
                }

                target.draw(tile);
            }
        }
    }

private:
    bool isCheck;
    bool isCheckmate;
    bool isStalemate;
};