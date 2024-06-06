#pragma once

#include "../Board/board.h"
#include "../Moves/MoveInstruction.hpp"
#include "../Serivces/SelectPieceService.hpp"
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
        int i = 0;
    }

private:
    Engine(Board* board, const GameState* gameState, int depth, PieceColor team) {
        this->gameState = new GameState(*gameState);
        this->board = new Board(*board, this->gameState);
        this->maxDepth = depth;
        this->team = team;
        this->originBoard = board;
    }

    ~Engine() {
        delete this->board;
        delete this->gameState;
    }

    bestMove start() {
        auto currentTeam = this->gameState->getCurrentTeam();
        MoveIndicator bestIndicator;
        Piece* piece;
        for (const auto& element: currentTeam) {
            SelectPieceService::Select(element, board);

            for (const auto& indicator: this->board->indicators) {

                GameState* gameStateCopy = new GameState(*this->board->gameState);
                Board* boardCopy = new Board(*this->board, gameStateCopy);
                int bestValue = INT_MIN;

                if (indicator->getMoveOption() == MoveOptions::Capture ||
                    indicator->getMoveOption() == MoveOptions::CAPTURE_AND_PROMOTION) {
                    CapturePieceService::Capture(
                            boardCopy,
                            boardCopy->getSelectedPieceReference(),
                            indicator,
                            Engine::getMock()
                    );
                } else {
                    MovePieceService::Move(
                            boardCopy,
                            boardCopy->getSelectedPieceReference(),
                            indicator,
                            Engine::getMock()
                    );
                }


                int boardValue = this->minimax(board, this->maxDepth - 1);

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

    int minimax(Board* board, int depth) {

        if (depth <= 0) {
            return this->evaluate(*board);
        }

        auto currentTeam = this->gameState->getCurrentTeam();

        for (const auto& element: currentTeam) {

            SelectPieceService::Select(element, board);

            if (board->indicators.empty()) {
                return this->evaluate(*board);
            }

            for (const auto& indicator: board->indicators) {

                GameState* gameStateCopy = new GameState(*board->gameState);
                Board* boardCopy = new Board(*board, gameStateCopy);
                int maxEval = INT_MIN;

                if (indicator->getMoveOption() == MoveOptions::Capture ||
                    indicator->getMoveOption() == MoveOptions::CAPTURE_AND_PROMOTION) {
                    CapturePieceService::Capture(
                            boardCopy,
                            boardCopy->getSelectedPieceReference(),
                            indicator,
                            Engine::getMock()
                    );
                } else {
                    MovePieceService::Move(
                            boardCopy,
                            boardCopy->getSelectedPieceReference(),
                            indicator,
                            Engine::getMock()
                    );
                }

                int eval = this->minimax(board, depth - 1);
                maxEval = std::max(maxEval, eval);
                delete gameStateCopy;
                delete boardCopy;

                return maxEval;
            }
        }
    }

    static void makeMoveOnRealBoard(const bestMove indicator, Board* board) {
        if (indicator.move.getMoveOption() == MoveOptions::Capture ||
            indicator.move.getMoveOption() == MoveOptions::CAPTURE_AND_PROMOTION) {
            CapturePieceService::Capture(
                    board,
                    indicator.piece,
                    &indicator.move,
                    Engine::getMock()
            );
        } else {
            MovePieceService::Move(
                    board,
                    indicator.piece,
                    &indicator.move,
                    Engine::getMock()
            );
        }
    }

    int evaluate(const Board& board) {
        int evaluation = 0;

        // Ewaluacja pionków
        Bitboard pawns = board.gameState->getBlackPawns();
        while (pawns) {
            int square = __builtin_ctzll(pawns); // Znajdź indeks pierwszego ustawionego bitu
            evaluation += 100; // Przykładowa wartość punktowa dla pionka
            pawns &= pawns - 1; // Wyczyść najmniej znaczący bit
        }

        // Ewaluacja skoczków
        Bitboard knights = board.gameState->getBlackKing();
        while (knights) {
            int square = __builtin_ctzll(knights); // Znajdź indeks pierwszego ustawionego bitu
            evaluation += 300; // Przykładowa wartość punktowa dla skoczka
            knights &= knights - 1; // Wyczyść najmniej znaczący bit
        }

        Bitboard bishops =  board.gameState->getBlackBishops();
        while (bishops) {
            int square = __builtin_ctzll(bishops); // Znajdź indeks pierwszego ustawionego bitu
            evaluation += 300; // Przykładowa wartość punktowa dla gońca
            bishops &= bishops - 1; // Wyczyść najmniej znaczący bit
        }

        // Ewaluacja wież
        Bitboard rooks =board.gameState->getBlackBishops();;
        while (rooks) {
            int square = __builtin_ctzll(rooks); // Znajdź indeks pierwszego ustawionego bitu
            evaluation += 500; // Przykładowa wartość punktowa dla wieży
            rooks &= rooks - 1; // Wyczyść najmniej znaczący bit
        }

        // Ewaluacja hetmanów
        Bitboard queens =board.gameState->getBlackQueen();;
        while (queens) {
            int square = __builtin_ctzll(queens); // Znajdź indeks pierwszego ustawionego bitu
            evaluation += 900; // Przykładowa wartość punktowa dla hetmana
            queens &= queens - 1; // Wyczyść najmniej znaczący bit
        }

        // Ewaluacja króla
        Bitboard kings = board.gameState->getBlackKing();
        while (kings) {
            int square = __builtin_ctzll(kings); // Znajdź indeks pierwszego ustawionego bitu
            evaluation += 10000; // Przykładowa wartość punktowa dla króla
            kings &= kings - 1; // Wyczyść najmniej znaczący bit
        }


        return evaluation;
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
    int maxDepth;
    PieceColor team;
};