#pragma once

#include "../EventListenerInterface.hpp"
#include "../../Assets/BitBoard.hpp"
#include "../../Serivces/CapurePieceService.hpp"
#include "../../Serivces/CheckMateService.hpp"

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
                            (indicator->getMoveOption() == MoveOptions::Capture ||
                             indicator->getMoveOption() == MoveOptions::CAPTURE_AND_PROMOTION)) {

                            CapturePieceService::Capture(
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