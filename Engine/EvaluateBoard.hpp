#pragma once

#include "../Board/board.h"
#include "Engine.hpp"

#define CHECK_BONUS 25
#define ATTACK_BONUS 5

class EvaluateBoard {
public:

    static int blackPawnTable[64];
    static int blackKnightTable[64];
    static int blackBishopTable[64];
    static int blackRookTable[64];
    static int blackQueenTable[64];
    static int blackKingTable[64];

    static int whitePawnTable[64];
    static int whiteKnightTable[64];
    static int whiteBishopTable[64];
    static int whiteRookTable[64];
    static int whiteQueenTable[64];
    static int whiteKingTable[64];

    static int evaluate(const Board* board) {
        int evaluation = 0;
        int evaluation1 = 0;

        evaluation = EvaluateBoard::evaluatePieces(board, PieceColor::BLACK_PIECE);
        evaluation1 = EvaluateBoard::evaluatePieces(board, PieceColor::WHITE_PIECE);

        evaluation+=EvaluateBoard::evaluateAttackBonus(board,PieceColor::BLACK_PIECE);
        evaluation1+=EvaluateBoard::evaluateAttackBonus(board,PieceColor::WHITE_PIECE);

        if (board->gameState->getTurn() == PieceColor::BLACK_PIECE) {
            if (board->gameState->getIsCheck()) {
                evaluation -= CHECK_BONUS;
            }
            if (board->gameState->getIsCheckMate()) {
                return INT_MIN;
            }
        }

        if (board->gameState->getTurn() == PieceColor::WHITE_PIECE) {
            if (board->gameState->getIsCheck()) {
                evaluation1 -= CHECK_BONUS;
            }
            if (board->gameState->getIsCheckMate()) {
                return INT_MAX;
            }
        }

        return evaluation - evaluation1;
    }

    static int evaluatePieces(const Board* board, PieceColor turnColor) {
        int evaluation = 0;

        evaluation += EvaluateBoard::evaluateSinglePiecePosition(board, PieceType::PAWN, turnColor);
        evaluation += EvaluateBoard::evaluateSinglePiecePosition(board, PieceType::KNIGHT, turnColor);
        evaluation += EvaluateBoard::evaluateSinglePiecePosition(board, PieceType::BISHOP, turnColor);
        evaluation += EvaluateBoard::evaluateSinglePiecePosition(board, PieceType::ROOK, turnColor);
        evaluation += EvaluateBoard::evaluateSinglePiecePosition(board, PieceType::QUEEN, turnColor);
        evaluation += EvaluateBoard::evaluateSinglePiecePosition(board, PieceType::KING, turnColor);

        return evaluation;
    }

    static PieceColor getOpponentColor(const PieceColor& color) {
        if (color == PieceColor::BLACK_PIECE) {
            return PieceColor::WHITE_PIECE;
        } else {
            return PieceColor::BLACK_PIECE;
        }
    }

    static int evaluateSinglePiecePosition(const Board* board, const PieceType& type, const PieceColor& color) {
        Bitboard position = board->gameState->getBitBoard(type, color);

        int evaluation = 0;

        for (int rank = 7; rank >= 0; --rank) {
            for (int file = 0; file < 8; ++file) {
                int shift = BitBoard::calcShift(file, rank);
                if (position & (1ULL << shift)) {
                    int* weightsTable = EvaluateBoard::getPieceWeights(type, color);
                    int weight = EvaluateBoard::getPieceWeight(type);

                    evaluation += weight + weightsTable[shift];  // Przykładowa wartość punktowa dla pionka

                }
            }
        }

        return evaluation;
    }

    static int* getPieceWeights(const PieceType& type, const PieceColor& color) {
        switch (type) {
            case PieceType::PAWN:
                return color == PieceColor::WHITE_PIECE ? EvaluateBoard::whitePawnTable : EvaluateBoard::blackPawnTable;
            case PieceType::KNIGHT:
                return color == PieceColor::WHITE_PIECE ? EvaluateBoard::whiteKnightTable
                                                        : EvaluateBoard::blackKnightTable;
            case PieceType::BISHOP:
                return color == PieceColor::WHITE_PIECE ? EvaluateBoard::whiteBishopTable
                                                        : EvaluateBoard::blackBishopTable;
            case PieceType::ROOK:
                return color == PieceColor::WHITE_PIECE ? EvaluateBoard::whiteRookTable : EvaluateBoard::blackRookTable;
            case PieceType::QUEEN:
                return color == PieceColor::WHITE_PIECE ? EvaluateBoard::whiteQueenTable
                                                        : EvaluateBoard::blackQueenTable;
            case PieceType::KING:
                return color == PieceColor::WHITE_PIECE ? EvaluateBoard::whiteKingTable : EvaluateBoard::blackKingTable;

        }
    }

    static int getPieceWeight(const PieceType& type) {
        switch (type) {
            case PieceType::PAWN:
                return 100;
            case PieceType::KNIGHT:
                return 310;
            case PieceType::BISHOP:
                return 330;
            case PieceType::ROOK:
                return 500;
            case PieceType::QUEEN:
                return 900;
            case PieceType::KING:
                return 20000;
            default:
                return 0;
        }
    }

    static int evaluateAttackBonus(const Board* board, const PieceColor& color) {

        int evaluation = 0;
        auto attack = board->gameState->getAttackedSquares(color);
        auto enemyBoard = board->gameState->calcBeatable(color);

        Bitboard attackedFields = enemyBoard & attack;

        for (int i = 0; i < 64; i++) {
            if (attackedFields & (1ULL << i)) {
                evaluation += ATTACK_BONUS;
            }
        }

        return evaluation;
    }

    friend class Engine;
};


int EvaluateBoard::blackPawnTable[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        5, -5, -10, 0, 0, -10, -5, 5,
        0, 0, 0, -10, -10, 0, 0, 0,
        5, 5, 10, 20, 20, 10, 5, 5,
        10, 10, 20, 40, 40, 20, 10, 10,
        30, 30, 40, 60, 60, 40, 30, 30,
        90, 90, 90, 90, 90, 90, 90, 90,
        0, 0, 0, 0, 0, 0, 0, 0
};

int EvaluateBoard::whitePawnTable[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        90, 90, 90, 90, 90, 90, 90, 90,
        30, 30, 40, 60, 60, 40, 30, 30,
        10, 10, 20, 40, 40, 20, 10, 10,
        5, 5, 10, 20, 20, 10, 5, 5,
        0, 0, 0, -10, -10, 0, 0, 0,
        5, -5, -10, 0, 0, -10, -5, 5,
        0, 0, 0, 0, 0, 0, 0, 0
};

int EvaluateBoard::blackKnightTable[64] = {
        -50, -40, -30, -30, -30, -30, -40, -50,
        -40, -20, 0, 0, 0, 0, -20, -40,
        -30, 5, 10, 15, 15, 10, 5, -30,
        -30, 5, 15, 20, 20, 15, 5, -30,
        -30, 5, 15, 20, 20, 15, 5, -30,
        -30, 5, 10, 15, 15, 10, 5, -30,
        -40, -20, 0, 5, 5, 0, -20, -40,
        -50, -40, -30, -30, -30, -30, -40, -50,
};

int EvaluateBoard::whiteKnightTable[64] = {
        -50, -40, -30, -30, -30, -30, -40, -50,
        -40, -20, 0, 5, 5, 0, -20, -40,
        -30, 5, 10, 15, 15, 10, 5, -30,
        -30, 5, 15, 20, 20, 15, 5, -30,
        -30, 5, 15, 20, 20, 15, 5, -30,
        -30, 5, 10, 15, 15, 10, 5, -30,
        -40, -20, 0, 0, 0, 0, -20, -40,
        -50, -40, -30, -30, -30, -30, -40, -50
};

int EvaluateBoard::blackBishopTable[64] = {
        -20, -10, -10, -10, -10, -10, -10, -20,
        -10, 5, 0, 0, 0, 0, 5, -10,
        -10, 10, 10, 10, 10, 10, 10, -10,
        -10, 0, 10, 15, 15, 10, 0, -10,
        -10, 5, 10, 15, 15, 10, 5, -10,
        -10, 0, 10, 10, 10, 10, 0, -10,
        -10, 0, 0, 0, 0, 0, 0, -10,
        -20, -10, -10, -10, -10, -10, -10, -20
};

int EvaluateBoard::whiteBishopTable[64] = {
        -20, -10, -10, -10, -10, -10, -10, -20,
        -10, 0, 0, 0, 0, 0, 0, -10,
        -10, 0, 5, 10, 10, 5, 0, -10,
        -10, 5, 5, 10, 10, 5, 5, -10,
        -10, 0, 10, 15, 15, 10, 0, -10,
        -10, 10, 10, 10, 10, 10, 10, -10,
        -10, 5, 0, 0, 0, 0, 5, -10,
        -20, -10, -10, -10, -10, -10, -10, -20
};

int EvaluateBoard::blackRookTable[64] = {
        0, 0, 0, 5, 5, 0, 0, 0,
        -5, 0, 0, 0, 0, 0, 0, -5,
        -5, 0, 0, 0, 0, 0, 0, -5,
        -5, 0, 0, 0, 0, 0, 0, -5,
        -5, 0, 0, 0, 0, 0, 0, -5,
        -5, 0, 0, 0, 0, 0, 0, -5,
        5, 20, 20, 20, 20, 20, 20, 5,
        0, 0, 0, 0, 0, 0, 0, 0,
};

int EvaluateBoard::whiteRookTable[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        5, 20, 20, 20, 20, 20, 20, 5,
        -5, 0, 0, 0, 0, 0, 0, -5,
        -5, 0, 0, 0, 0, 0, 0, -5,
        -5, 0, 0, 0, 0, 0, 0, -5,
        -5, 0, 0, 0, 0, 0, 0, -5,
        -5, 0, 0, 0, 0, 0, 0, -5,
        0, 0, 0, 5, 5, 0, 0, 0
};

int EvaluateBoard::blackQueenTable[64] = {
        -20, -10, -10, -5, -5, -10, -10, -20,
        -10, 0, 5, 0, 0, 0, 0, -10,
        -10, 5, 5, 5, 5, 5, 0, -10,
        -5, 0, 5, 10, 10, 5, 0, -5,
        -5, 0, 5, 10, 10, 5, 0, -5,
        -10, 0, 5, 5, 5, 5, 0, -10,
        -10, 0, 0, 0, 0, 0, 0, -10,
        -20, -10, -10, -5, -5, -10, -10, -20
};

int EvaluateBoard::whiteQueenTable[64] = {
        -20, -10, -10, -5, -5, -10, -10, -20,
        -10, 0, 0, 0, 0, 0, 0, -10,
        -10, 0, 5, 5, 5, 5, 0, -10,
        -5, 0, 5, 10, 10, 5, 0, -5,
        -5, 0, 5, 10, 10, 5, 0, -5,
        -10, 0, 5, 5, 5, 5, 0, -10,
        -10, 0, 0, 0, 0, 0, 0, -10,
        -20, -10, -10, -5, -5, -10, -10, -20
};

int EvaluateBoard::blackKingTable[64] = {
        -50, -40, -30, -20, -20, -30, -40, -50,
        -30, -20, -10, 0, 0, -10, -20, -30,
        -30, -10, 20, 30, 30, 20, -10, -30,
        -30, -10, 30, 40, 40, 30, -10, -30,
        -30, -10, 30, 40, 40, 30, -10, -30,
        -30, -10, 20, 30, 30, 20, -10, -30,
        -30, -30, 0, 0, 0, 0, -30, -30,
        -50, -30, -30, -30, -30, -30, -30, -50
};

int EvaluateBoard::whiteKingTable[64] = {
        -50, -30, -30, -30, -30, -30, -30, -50,
        -30, -30, 0, 0, 0, 0, -30, -30,
        -30, -10, 20, 30, 30, 20, -10, -30,
        -30, -10, 30, 40, 40, 30, -10, -30,
        -30, -10, 30, 40, 40, 30, -10, -30,
        -30, -10, 20, 30, 30, 20, -10, -30,
        -30, -20, -10, 0, 0, -10, -20, -30,
        -50, -40, -30, -20, -20, -30, -40, -50
};