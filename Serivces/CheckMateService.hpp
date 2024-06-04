#pragma once

#include "../Assets/BitBoard.hpp"
#include "../Board/board.h"
#include "../Serivces/SelectPieceService.hpp"
#include "../Engine/MoveSimulator.hpp"

class CheckEscape {
public:
    static void getCheckEscapeMoves(const Board* board, GameState* gameState) {
        CheckEscape* escape = new CheckEscape(board, gameState);
        auto moves = escape->findMoves();
        BitBoard::dump(moves, "esc");
        gameState->checkEscapeMoves = moves;
    }

private:

    CheckEscape(const Board* board, const GameState* gameState) {

        this->gameStateEntryCopy = new GameState(*gameState);
        this->boardEntryCopy = new Board(*board, this->gameStateEntryCopy);
    }

    ~CheckEscape() {
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