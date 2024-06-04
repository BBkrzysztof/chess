#pragma once

#include "../Board/board.h"
#include "../Moves/Move.hpp"
#include "../Moves/Promote.hpp"
#include "../Moves/Capture.hpp"
#include "../Assets/PopUp.hpp"

class CapturePieceService {
public:
    static void Capture(Board* board, Piece* selectedPiece, const MoveIndicator* indicator, PopUpInterface* popUp = nullptr) {
        CapturePieceService* service = new CapturePieceService(board, board->gameState);
        service->capture(selectedPiece, indicator, popUp);
    }

private:
    CapturePieceService(Board* board, GameState* gameState) {
        this->board = board;
        this->gameState = gameState;
    }

    void capture(Piece* selectedPiece, const MoveIndicator* indicator, PopUpInterface* popUp) {

        MoveInstruction instruction;
        instruction.board = this->board;
        instruction.gameState = this->gameState;
        instruction.selectedPiece = selectedPiece;

        this->gameState->markFirstMove(selectedPiece);
        instruction.oldX = indicator->getPositionX();
        instruction.oldY = indicator->getPositionY();
        Capture::capture(instruction);


        if (indicator->getMoveOption() == MoveOptions::CAPTURE_AND_PROMOTION) {
            instruction.newX = indicator->getPositionX();
            instruction.newY = indicator->getPositionY();
            instruction.selectedType = popUp->draw();

            Promote::promote(instruction);

        } else {
            instruction.newX = indicator->getPositionX();
            instruction.newY = indicator->getPositionY();
            instruction.captured = true;
            Move::move(instruction);
        }
    }

private:
    Board* board;
    GameState* gameState;
};