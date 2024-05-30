#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

#include "../EventListenerInterface.hpp"

class MovePieceListener : public EventListenerInterface {
    void onEvent(sf::Event event) final {
        if (this->board->getSelectedPiece() != "") {
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(*this->window);
                    auto selectedPiece = this->board->getSelectedPieceReference();
                    int x = (mousePosition.x / 100) * 100;
                    int y = (mousePosition.y / 100) * 100;
                    if (selectedPiece->getPositionX() != x && selectedPiece->getPositionY() != y) {
                        selectedPiece->move(x, y);
                    }
                }
            }
        }
    }
};