#pragma once

#include "../Assets/BitBoard.hpp"
#include "../Board/board.h"
#include "SelectPieceService.hpp"
#include "../Engine/MoveSimulator.hpp"

class CheckMateService {
public:
    static void checkMate(const Board* board, GameState* gameState) {
        CheckMateService* escape = new CheckMateService(board, gameState);
        auto moves = escape->findMoves();
        gameState->setIsCheckMate(moves == 0ULL);
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

    Bitboard findMoves() {
        Bitboard escapeMove = 0ULL;

        auto team = this->gameStateEntryCopy->getCurrentTeam();

        for (Piece* piece: team) {
            //select each piece
            this->boardEntryCopy->setSelectedPiece(piece->getHash());
            SelectPieceService::Select(piece, this->boardEntryCopy);

            for (const auto& indicator: this->boardEntryCopy->indicators) {
                //simulate each move
                bool isCheck = MoveSimulator::simulateMoveAndCheckIsCheck(
                        indicator,
                        this->gameStateEntryCopy,
                        this->boardEntryCopy
                );
                if (!isCheck) {
                    int shift = BitBoard::calcShift(indicator->getPositionX(), indicator->getPositionY());
                    escapeMove |= BitBoard::insert(0ULL, shift);
                }
            }
        }
        delete this->boardEntryCopy;
        delete this->gameStateEntryCopy;
        return escapeMove;
    }

private:
    GameState* gameStateEntryCopy;
    Board* boardEntryCopy;

};