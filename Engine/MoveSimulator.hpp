#pragma once

#include "../Assets/BitBoard.hpp"
#include "../Board/board.h"
#include "../Serivces/MovePieceService.hpp"
#include "../Serivces/CapurePieceService.hpp"

class MoveSimulator {
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
                    MoveSimulator::getMock()
            );
        } else {
            MovePieceService::Move(
                    boardCopy,
                    boardCopy->getSelectedPieceReference(),
                    indicator,
                    MoveSimulator::getMock()
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