#pragma once

#include "../Board/board.h"
#include "../Moves/Move.hpp"
#include "../Moves/Promote.hpp"
#include "../Moves/Castle.hpp"
#include "../Assets/PopUp.hpp"

class MovePieceService {
public:
    static void
    Move(Board* board, Piece* selectedPiece, const MoveIndicator* indicator, PopUpInterface* popUp = nullptr) {
        MovePieceService* service = new MovePieceService(board, board->gameState);
        service->move(selectedPiece, indicator, popUp);
    }

private:
    MovePieceService(Board* board, GameState* gameState) {
        this->board = board;
        this->gameState = gameState;
    }

    void move(Piece* selectedPiece, const MoveIndicator* indicator, PopUpInterface* popUp) {
        MoveInstruction instruction;
        instruction.board = this->board;
        instruction.gameState = this->gameState;
        instruction.selectedPiece = selectedPiece;
        instruction.pieceColor = selectedPiece->getPieceColor();

        if (indicator->getMoveOption() == MoveOptions::Move) {
            this->gameState->markFirstMove(selectedPiece);
            instruction.newX = indicator->getPositionX();
            instruction.newY = indicator->getPositionY();

            Move::move(instruction);

        } else if (indicator->getMoveOption() == MoveOptions::PROMOTION) {
            instruction.newX = indicator->getPositionX();
            instruction.newY = indicator->getPositionY();
            instruction.selectedType = popUp->draw();
            Promote::promote(instruction);
        } else if (
                indicator->getMoveOption() == MoveOptions::KING_SIDE_CASTLE ||
                indicator->getMoveOption() == MoveOptions::QUEEN_SIDE_CASTLE
                ) {
            instruction.options = indicator->getMoveOption();
            Castle::castle(instruction);
        }
    }

private:
    Board* board;
    GameState* gameState;
};