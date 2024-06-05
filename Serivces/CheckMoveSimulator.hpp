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

        if (indicator->getMoveOption() == MoveOptions::Capture ||
            indicator->getMoveOption() == MoveOptions::CAPTURE_AND_PROMOTION) {
            CapturePieceService::Capture(
                    boardCopy,
                    boardCopy->getSelectedPieceReference(),
                    indicator,
                    CheckMoveSimulator::getMock()
            );
        } else {
            MovePieceService::Move(
                    boardCopy,
                    boardCopy->getSelectedPieceReference(),
                    indicator,
                    CheckMoveSimulator::getMock()
            );
        }
        gameStateCopy->toggleTurn();
        bool result = boardCopy->isCheck();

        delete boardCopy;
        delete gameStateCopy;

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