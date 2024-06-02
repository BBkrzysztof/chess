#pragma once

#include "../EventListenerInterface.hpp"
#include "../../Assets/BitBoard.hpp"

class CapturePieceListener : public EventListenerInterface {
    void onEvent(sf::Event event) final {
        if (this->board->getSelectedPiece() != "") {
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(*this->window);
                    auto indicators = this->board->getIndicators();
                    auto selectedPiece = this->board->getSelectedPieceReference();

                    for (const auto& indicator: indicators) {
                        if (indicator->validateBounds(mousePosition) &&
                            indicator->getMoveOption() == MoveOptions::Capture) {
                            this->board->capture(
                                    indicator->getPositionX(),
                                    indicator->getPositionY()
                            );
                            this->board->move(
                                    selectedPiece,
                                    indicator->getPositionX(),
                                    indicator->getPositionY(),
                                    true
                            );
                        }
                    }
                }
            }
        }
    }
};