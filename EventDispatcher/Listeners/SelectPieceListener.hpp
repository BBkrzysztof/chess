#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

#include "../EventListenerInterface.hpp"
#include "../../Assets/BitBoard.hpp"
#include "../../Serivces/SelectPieceService.hpp"

class SelectPieceListener : public EventListenerInterface {
    void onEvent(sf::Event event) final {

        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(*this->window);
                auto pieces = this->board->getPieces();

                for (const auto& element: pieces) {
                    if (element.second->getPieceColor() == this->gameState->getTurn() &&
                        element.second->validateBounds(mousePosition)) {

                        if (this->board->getSelectedPiece() == element.first) {
                            this->board->setSelectedPiece("");
                            this->board->clearIndicators();
                        } else {
                            SelectPieceService::Select(element.second, this->board);
                        }
                    }
                }
            }
        }
    }

};