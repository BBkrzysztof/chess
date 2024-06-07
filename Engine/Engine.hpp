#pragma once

#include "../Board/board.h"
#include "../Moves/MoveInstruction.hpp"
#include "../Serivces/SelectPieceService.hpp"
#include "EvaluateBoard.hpp"

#include <thread>

struct bestMove {
    Piece* piece;
    MoveIndicator move;
};

class Engine {
public:
    static void run(Board* board, GameState* gameState, PieceColor team, int depth = 4) {
        Engine* engine = new Engine(board, gameState, depth, team);
        bestMove indicator = engine->start();
        Engine::makeMoveOnRealBoard(indicator, board);
        delete engine;
    }

private:
    Engine(Board* board, const GameState* gameState, int depth, PieceColor team) {
        this->gameState = new GameState(*gameState);
        this->board = new Board(*board, this->gameState);
        this->maxDepth = depth;
        this->team = team;
        this->originBoard = board;
        this->popUp = Engine::getMock();
    }

    ~Engine() {
        delete this->board;
        delete this->gameState;
        delete this->popUp;
    }

    bestMove start() {
        auto currentTeam = this->gameState->getCurrentTeam();
        MoveIndicator bestIndicator;
        Piece* piece = nullptr;
        int bestValue = INT_MIN;

        GameState* gameStateEntryCopy = new GameState(*board->gameState);
        Board* boardEntryCopy = new Board(*this->board, gameStateEntryCopy, this->board->getIndicators());

        for (const auto& element: currentTeam) {
            SelectPieceService::Select(element, boardEntryCopy, true);


            for (const auto& indicator: boardEntryCopy->indicators) {

                GameState* gameStateCopy = new GameState(*gameStateEntryCopy);
                Board* boardCopy = new Board(*boardEntryCopy, gameStateCopy);

                if (indicator->getMoveOption() == MoveOptions::Capture ||
                    indicator->getMoveOption() == MoveOptions::CAPTURE_AND_PROMOTION) {
                    CapturePieceService::Capture(
                            boardCopy,
                            boardCopy->getSelectedPieceReference(),
                            indicator,
                            this->popUp
                    );
                } else {
                    MovePieceService::Move(
                            boardCopy,
                            boardCopy->getSelectedPieceReference(),
                            indicator,
                            this->popUp
                    );
                }


                int boardValue = this->minimax(boardCopy, this->maxDepth - 1);

                if (boardValue > bestValue) {
                    bestValue = boardValue;
                    bestIndicator = indicator;
                    piece = element;
                }
                delete boardCopy;
                delete gameStateCopy;
            }
        }

        Piece* realPiece = this->originBoard->pieces[piece->getHash()];

        delete boardEntryCopy;
        delete gameStateEntryCopy;

        return {realPiece, bestIndicator};
    }

    int minimax(Board* board, int depth) {

        auto color = board->gameState->getTurn();

        GameState* gameStateEntryCopy = new GameState(*board->gameState);
        Board* boardEntryCopy = new Board(*board, gameStateEntryCopy);


        if (depth <= 0) {
            return EvaluateBoard::evaluate(boardEntryCopy, color);
        }

        auto currentTeam = boardEntryCopy->gameState->getCurrentTeam();
        int maxEval = INT_MIN;
        int minEval = INT_MAX;
        for (const auto& element: currentTeam) {

            SelectPieceService::Select(element, boardEntryCopy, true);

            if (boardEntryCopy->indicators.empty()) {
                return EvaluateBoard::evaluate(boardEntryCopy, color);
            }


            for (const auto& indicator: boardEntryCopy->indicators) {

                GameState* gameStateCopy = new GameState(*boardEntryCopy->gameState);
                Board* boardCopy = new Board(*boardEntryCopy, gameStateCopy);


                if (indicator->getMoveOption() == MoveOptions::Capture ||
                    indicator->getMoveOption() == MoveOptions::CAPTURE_AND_PROMOTION) {
                    CapturePieceService::Capture(
                            boardCopy,
                            boardCopy->getSelectedPieceReference(),
                            indicator,
                            this->popUp
                    );
                } else {
                    MovePieceService::Move(
                            boardCopy,
                            boardCopy->getSelectedPieceReference(),
                            indicator,
                            this->popUp
                    );
                }

                int eval = this->minimax(board, depth - 1);

                delete boardCopy;
                delete gameStateCopy;

                if (color == this->team) {
                    maxEval = std::max(maxEval, eval);
                } else {
                    minEval = std::min(minEval, eval);
                }
            }
        }

        delete boardEntryCopy;
        delete gameStateEntryCopy;

        return color == this->team ? maxEval : minEval;
    }

    static void makeMoveOnRealBoard(const bestMove indicator, Board* board) {
        auto* mock = Engine::getMock();
        if (indicator.move.getMoveOption() == MoveOptions::Capture ||
            indicator.move.getMoveOption() == MoveOptions::CAPTURE_AND_PROMOTION) {
            CapturePieceService::Capture(
                    board,
                    indicator.piece,
                    &indicator.move,
                    mock
            );
        } else {
            MovePieceService::Move(
                    board,
                    indicator.piece,
                    &indicator.move,
                    mock
            );
        }

        delete mock;
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
    Board* board;
    Board* originBoard;
    GameState* gameState;
    PopUpInterface* popUp;
    int maxDepth;
    PieceColor team;
};