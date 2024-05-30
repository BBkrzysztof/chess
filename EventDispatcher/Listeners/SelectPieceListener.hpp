#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

#include "../EventListenerInterface.hpp"

class SelectPieceListener : public EventListenerInterface {
    void onEvent(sf::Event event) final {

        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(*this->window);
                auto pieces = this->board->getPieces();

                for (const auto& element: pieces) {
                    if (element.second->validateBounds(mousePosition)) {

                        if (this->board->getSelectedPiece() == element.first) {
                            this->board->setSelectedPiece("");
                        } else {
                            this->board->setSelectedPiece(element.first);
                        }
                    }
                }
            }
        }
    }
};