#pragma once

#include "../Assets/BitBoard.hpp"
#include "../Board/board.h"
#include "SelectPieceService.hpp"
#include "CheckMoveSimulator.hpp"

class CheckMateService {
public:
    static void checkMate(const Board* board, GameState* gameState) {
        if (gameState->getIsCheck()) {
            auto* escape = new CheckMateService(board, gameState);
            auto isMat = escape->validate();
            gameState->setIsCheckMate(isMat);
        }
    }

private:

    CheckMateService(const Board* board, const GameState* gameState) {

        this->gameStateEntryCopy = new GameState(*gameState);
        this->boardEntryCopy = new Board(*board, this->gameStateEntryCopy);
    }

    ~CheckMateService() {
        delete this->boardEntryCopy;
        delete this->gameStateEntryCopy;
    }

    bool validate() {
        bool isMat = true;

        auto team = this->gameStateEntryCopy->getCurrentTeam();

        for (Piece* piece: team) {
            //select each piece
            this->boardEntryCopy->setSelectedPiece(piece->getHash());
            SelectPieceService::Select(piece, this->boardEntryCopy);

            if (!this->boardEntryCopy->getIndicators().empty()) {
                isMat = false;
                break;
            }
        }

        return isMat;
    }

private:
    GameState* gameStateEntryCopy;
    Board* boardEntryCopy;

};