#pragma once


#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

#include "../EventListenerInterface.hpp"
#include "../../Assets/BitBoard.hpp"
#include "../../Assets/PopUp.hpp"

class MovePieceListener : public EventListenerInterface {
    void onEvent(sf::Event event) final {
        if (this->board->getSelectedPiece() != "") {
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(*this->window);
                    auto selectedPiece = this->board->getSelectedPieceReference();

                    auto indicators = this->board->getIndicators();

                    for (const auto& indicator: indicators) {
                        if (indicator->validateBounds(mousePosition)) {
                            if (indicator->getMoveOption() == MoveOptions::Move) {
                                this->gameState->markFirstMove(selectedPiece);
                                this->board->move(
                                        selectedPiece,
                                        indicator->getPositionX(),
                                        indicator->getPositionY()
                                );
                            } else if (indicator->getMoveOption() == MoveOptions::PROMOTION) {
                                auto selectedPieceType = PopUp(selectedPiece->getPieceColor()).draw();
                                this->board->promote(
                                        selectedPiece,
                                        selectedPieceType,
                                        indicator->getPositionX(),
                                        indicator->getPositionY()
                                );
                            } else if (
                                    indicator->getMoveOption() == MoveOptions::KING_SIDE_CASTLE ||
                                    indicator->getMoveOption() == MoveOptions::QUEEN_SIDE_CASTLE
                                    ) {

                                this->board->castle(
                                        indicator->getMoveOption(),
                                        selectedPiece->getPieceColor()
                                );
                            }
                        }
                    }
                }
            }
        }
    }
};