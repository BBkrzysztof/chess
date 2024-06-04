#pragma once

#include "../Assets/BitBoard.hpp"
#include "../Board/board.h"
#include "../Serivces/SelectPieceService.hpp"
#include "../Serivces/MovePieceService.hpp"
#include "../Serivces/CapurePieceService.hpp"

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
                bool isCheck = this->simulateMove(indicator);
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

    bool simulateMove(MoveIndicator* indicator) {

        GameState* gameStateCopy = new GameState(*this->gameStateEntryCopy);
        Board* boardCopy = new Board(*this->boardEntryCopy, gameStateCopy);

        if (indicator->getMoveOption() == MoveOptions::Capture ||
            indicator->getMoveOption() == MoveOptions::CAPTURE_AND_PROMOTION) {
            CapturePieceService::Capture(
                    boardCopy,
                    boardCopy->getSelectedPieceReference(),
                    indicator,
                    CheckEscape::getMock()
            );
        } else {
            MovePieceService::Move(
                    boardCopy,
                    boardCopy->getSelectedPieceReference(),
                    indicator,
                    CheckEscape::getMock()
            );
        }

        bool result = boardCopy->amICheck(true);

        delete gameStateCopy;
        delete boardCopy;

        return result;
    }

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

private:
    GameState* gameStateEntryCopy;
    Board* boardEntryCopy;

};