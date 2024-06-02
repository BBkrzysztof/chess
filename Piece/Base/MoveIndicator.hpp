#pragma once

#include <SFML/Graphics.hpp>

sf::Color gray(176, 196, 222);
sf::Color capture(250, 245, 160);

enum MoveOptions {
    Move = 1,
    Capture = 2,
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
        } else {
            this->circleShape.setRadius(50);
            this->circleShape.setFillColor(capture);
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