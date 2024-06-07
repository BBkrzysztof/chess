#pragma once

#include "../Assets/BitBoard.hpp"
#include "../Board/board.h"
#include "MovePieceService.hpp"
#include "CapurePieceService.hpp"

class CheckMoveSimulator {
public:
    static bool simulateMoveAndCheckIsCheck(MoveIndicator* indicator, const GameState* gameState, const Board* board) {

        GameState* gameStateCopy = new GameState(*gameState);
        Board* boardCopy = new Board(*board, gameStateCopy);

        auto* mock = CheckMoveSimulator::getMock();

        if (indicator->getMoveOption() == MoveOptions::Capture ||
            indicator->getMoveOption() == MoveOptions::CAPTURE_AND_PROMOTION) {
            CapturePieceService::Capture(
                    boardCopy,
                    boardCopy->getSelectedPieceReference(),
                    indicator,
                    mock
            );
        } else {
            MovePieceService::Move(
                    boardCopy,
                    boardCopy->getSelectedPieceReference(),
                    indicator,
                    mock
            );
        }
        gameStateCopy->toggleTurn();
        bool result = boardCopy->isCheck();

        delete boardCopy;
        delete gameStateCopy;
        delete mock;
        return result;
    }

private:
    static PopUpInterface* getMock() {
        class Mock : public PopUpInterface {
        public:
            Mock(PieceType type) : type(type) {}

            PieceType draw() {
                return this->type;
            }

        private:
            PieceType type;
        };

        return new Mock(PieceType::QUEEN);
    }
};