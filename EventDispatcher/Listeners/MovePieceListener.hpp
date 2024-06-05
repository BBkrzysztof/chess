#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

#include "../EventListenerInterface.hpp"
#include "../../Assets/BitBoard.hpp"
#include "../../Assets/PopUp.hpp"
#include "../../Serivces/MovePieceService.hpp"
#include "../../Serivces/CheckMateService.hpp"

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
                            MovePieceService::Move(
                                    this->board,
                                    selectedPiece,
                                    indicator,
                                    new PopUp(selectedPiece->getPieceColor())
                            );
                            this->board->isCheck();
                            CheckMateService::checkMate(this->board, this->gameState);
                        }
                    }
                }
            }
        }
    }
};