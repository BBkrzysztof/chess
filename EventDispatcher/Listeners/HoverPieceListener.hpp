#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

#include "../EventListenerInterface.hpp"

class HoverPieceListener : public EventListenerInterface {
public:
    HoverPieceListener() {
        this->pointer.loadFromSystem(sf::Cursor::Hand);
        this->arrow.loadFromSystem(sf::Cursor::Arrow);
    }

private:
    void onEvent(sf::Event event) final {

        sf::Vector2i mousePosition = sf::Mouse::getPosition(*this->window);
        auto pieces = this->board->getPieces();

        for (const auto& element: pieces) {
            if (element.second->validateBounds(mousePosition)) {
                this->window->setMouseCursor(this->pointer);
            } else {
                this->window->setMouseCursor(this->arrow);
            }
        }
    }

private:
    sf::Cursor pointer;
    sf::Cursor arrow;
};