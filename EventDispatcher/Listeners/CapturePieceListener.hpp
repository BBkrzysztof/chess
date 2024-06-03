#pragma once

#include "../EventListenerInterface.hpp"
#include "../../Assets/BitBoard.hpp"
#include "../../Moves/Capture.hpp"
#include "../../Moves/Move.hpp"
#include "../../Moves/Promote.hpp"

class CapturePieceListener : public EventListenerInterface {
    void onEvent(sf::Event event) final {
        if (this->board->getSelectedPiece() != "") {
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(*this->window);
                    auto indicators = this->board->getIndicators();
                    auto selectedPiece = this->board->getSelectedPieceReference();

                    MoveInstruction instruction;
                    instruction.board = this->board;
                    instruction.gameState = this->gameState;
                    instruction.selectedPiece = selectedPiece;

                    for (const auto& indicator: indicators) {
                        if (indicator->validateBounds(mousePosition) &&
                            (indicator->getMoveOption() == MoveOptions::Capture ||
                             indicator->getMoveOption() == MoveOptions::CAPTURE_AND_PROMOTION)) {
                            this->gameState->markFirstMove(selectedPiece);
                            instruction.oldX = indicator->getPositionX();
                            instruction.oldY = indicator->getPositionY();
                            Capture::capture(instruction);


                            if (indicator->getMoveOption() == MoveOptions::CAPTURE_AND_PROMOTION) {
                                instruction.newX = indicator->getPositionX();
                                instruction.newY = indicator->getPositionY();
                                instruction.selectedType = PopUp(selectedPiece->getPieceColor()).draw();

                                Promote::promote(instruction);

                            } else {
                                instruction.newX = indicator->getPositionX();
                                instruction.newY = indicator->getPositionY();
                                instruction.captured = true;
                                Move::move(instruction);

                            }
                        }
                    }
                }
            }
        }
    }
};