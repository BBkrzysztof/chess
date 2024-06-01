#pragma once

#include "../EventListenerInterface.hpp"
#include "../../Assets/BitBoard.hpp"

class MovePieceListener : public EventListenerInterface {
    void onEvent(sf::Event event) final {
        if (this->board->getSelectedPiece() != "") {
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(*this->window);
                    auto selectedPiece = this->board->getSelectedPieceReference();

                    auto indicators = this->board->getIndicators();

                    for (const auto& indicator: indicators) {
                        if (indicator->validateBounds(mousePosition) &&
                            indicator->getMoveOption() == MoveOptions::Capture) {
                            this->board->move(
                                    selectedPiece,
                                    indicator->getPositionX(),
                                    indicator->getPositionY()
                            );
                        }
                    }

                }
            }
        }
    }
};