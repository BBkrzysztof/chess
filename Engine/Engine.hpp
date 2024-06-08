#pragma once

#include <queue>

#include "../Board/board.h"
#include "../Moves/MoveInstruction.hpp"
#include "../Serivces/SelectPieceService.hpp"
#include "EvaluateBoard.hpp"

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


        for (const auto& element: currentTeam) {
            SelectPieceService::Select(element, this->board, true);


            for (const auto& indicator: this->board->indicators) {

                GameState* gameStateCopy = new GameState(*this->gameState);
                Board* boardCopy = new Board(*this->board, gameStateCopy);

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

                boardCopy->isCheck();
                CheckMateService::checkMate(boardCopy, gameStateCopy);

                int boardValue = this->minimax(boardCopy, this->maxDepth - 1, INT_MIN, INT_MAX, false);
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

        return {realPiece, bestIndicator};
    }

    int minimax(Board* board, int depth, int alpha, int beta, bool maximizingPlayer) {

        if (depth == 0 || this->board->gameState->getIsCheckMate()) {

            int eval = EvaluateBoard::evaluate(board);

            return eval;
        }

        auto currentTeam = board->gameState->getCurrentTeam();
        if (maximizingPlayer) {
            int maxEval = INT_MIN;
            for (const auto& element: currentTeam) {

                SelectPieceService::Select(element, board, true);

                if (board->indicators.empty()) {
                    int eval = EvaluateBoard::evaluate(board);

                    return eval;
                }


                for (auto& indicator: board->indicators) {

                    GameState* gameStateCopy = new GameState(*board->gameState);
                    Board* boardCopy = new Board(*board, gameStateCopy);


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

                    boardCopy->isCheck();
                    CheckMateService::checkMate(boardCopy, gameStateCopy);

                    int eval = this->minimax(boardCopy, depth - 1, alpha, beta, false);

                    delete boardCopy;
                    delete gameStateCopy;

                    maxEval = std::max(maxEval, eval);
                    alpha = std::max(alpha, eval);
                    if (beta <= alpha) {
                        break;
                    }
                }
            }

            return maxEval;
        } else {
            int minEval = INT_MAX;
            for (const auto& element: currentTeam) {

                SelectPieceService::Select(element, board, true);

                for (const auto& indicator: board->indicators) {

                    GameState* gameStateCopy = new GameState(*board->gameState);
                    Board* boardCopy = new Board(*board, gameStateCopy);


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

                    boardCopy->isCheck();
                    CheckMateService::checkMate(boardCopy, gameStateCopy);

                    int eval = this->minimax(boardCopy, depth - 1, alpha, beta, true);

                    delete boardCopy;
                    delete gameStateCopy;

                    minEval = std::min(minEval, eval);
                    beta = std::min(beta, eval);
                    if (beta <= alpha) {
                        break;
                    }
                }
            }

            return minEval;
        }

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


        board->isCheck();
        CheckMateService::checkMate(board, board->gameState);

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

