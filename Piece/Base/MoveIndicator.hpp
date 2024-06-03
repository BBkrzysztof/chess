#pragma once

#include <SFML/Graphics.hpp>

sf::Color gray(176, 196, 222);
sf::Color capture(250, 245, 160);
sf::Color castle(173, 216, 230);
sf::Color promotion(100, 216, 230);

enum MoveOptions {
    Move = 1,
    Capture = 2,
    QUEEN_SIDE_CASTLE = 3,
    KING_SIDE_CASTLE = 4,
    PROMOTION = 5,
    CAPTURE_AND_PROMOTION = 6,
};

class MoveIndicator {
public:
    MoveIndicator(int x, int y, MoveOptions option = MoveOptions::Move) {
        this->positionX = x;
        this->positionY = y;

        if (option == MoveOptions::Move) {
            this->circleShape.setRadius(25);
            this->circleShape.setFillColor(gray);
            this->circleShape.setPosition(x * 100 + 25, y * 100 + 25);
        } else if (option == MoveOptions::PROMOTION) {
            this->circleShape.setRadius(25);
            this->circleShape.setFillColor(promotion);
            this->circleShape.setPosition(x * 100 + 25, y * 100 + 25);
        } else {
            this->circleShape.setRadius(50);
            if (option == MoveOptions::QUEEN_SIDE_CASTLE || option == MoveOptions::KING_SIDE_CASTLE) {
                this->circleShape.setFillColor(castle);
            } else {
                this->circleShape.setFillColor(capture);
            }
            this->circleShape.setPosition(x * 100, y * 100);
        }
        this->moveOption = option;
    };

    void draw(sf::RenderTarget& target) {
        target.draw(this->circleShape);
    }

    bool validateBounds(const sf::Vector2i& mousePosition) {
        sf::FloatRect bounds = this->circleShape.getGlobalBounds();
        return bounds.contains(static_cast<sf::Vector2f>(mousePosition));
    }

    int getPositionX() {
        return this->positionX;
    }

    int getPositionY() {
        return this->positionY;
    }

    MoveOptions getMoveOption() {
        return this->moveOption;
    }

private:
    int positionX = 0;
    int positionY = 0;
    sf::CircleShape circleShape;
    MoveOptions moveOption;
};