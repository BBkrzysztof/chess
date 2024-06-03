#pragma once


#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>

#include "../EventListenerInterface.hpp"
#include "../../Assets/BitBoard.hpp"
#include "../../Assets/PopUp.hpp"
#include "../../Moves/Move.hpp"
#include "../../Moves/Promote.hpp"
#include "../../Moves/Castle.hpp"

class MovePieceListener : public EventListenerInterface {
    void onEvent(sf::Event event) final {
        if (this->board->getSelectedPiece() != "") {
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePosition = sf::Mouse::getPosition(*this->window);
                    auto selectedPiece = this->board->getSelectedPieceReference();
                    auto indicators = this->board->getIndicators();

                    MoveInstruction instruction;
                    instruction.board = this->board;
                    instruction.gameState = this->gameState;
                    instruction.selectedPiece = selectedPiece;

                    for (const auto& indicator: indicators) {
                        if (indicator->validateBounds(mousePosition)) {
                            if (indicator->getMoveOption() == MoveOptions::Move) {
                                this->gameState->markFirstMove(selectedPiece);
                                instruction.newX = indicator->getPositionX();
                                instruction.newY = indicator->getPositionY();

                                Move::move(instruction);
                            } else if (indicator->getMoveOption() == MoveOptions::PROMOTION) {
                                instruction.newX = indicator->getPositionX();
                                instruction.newY = indicator->getPositionY();
                                instruction.selectedType = PopUp(selectedPiece->getPieceColor()).draw();
                                Promote::promote(instruction);
                            } else if (
                                    indicator->getMoveOption() == MoveOptions::KING_SIDE_CASTLE ||
                                    indicator->getMoveOption() == MoveOptions::QUEEN_SIDE_CASTLE
                                    ) {
                                instruction.options = indicator->getMoveOption();
                                Castle::castle(instruction);
                            }
                        }
                    }
                }
            }
        }
    }
};