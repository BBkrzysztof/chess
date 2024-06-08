#pragma once

#include "../Board/board.h"
#include "Engine.hpp"

class EvaluateBoard {
private:

    static int pawnTable[64];
    static int knightTable[64];
    static int bishopTable[64];
    static int rookTable[64];
    static int queenTable[64];
    static int kingTable[64];


    static int evaluate(const Board* board, PieceColor turnColor) {
        int evaluation = 0;
        Bitboard pawns = board->gameState->getBitBoard(PieceType::PAWN, turnColor);
        Bitboard knights = board->gameState->getBitBoard(PieceType::KNIGHT, turnColor);
        Bitboard bishops = board->gameState->getBitBoard(PieceType::BISHOP, turnColor);
        Bitboard rooks = board->gameState->getBitBoard(PieceType::ROOK, turnColor);
        Bitboard queens = board->gameState->getBitBoard(PieceType::QUEEN, turnColor);
        Bitboard kings = board->gameState->getBitBoard(PieceType::KING, turnColor);


        // Ewaluacja pionków
        while (pawns) {
            int square = __builtin_ctzll(pawns); // Znajdź indeks pierwszego ustawionego bitu
            evaluation += 100 + EvaluateBoard::pawnTable[square]; // Przykładowa wartość punktowa dla pionka
            pawns &= pawns - 1; // Wyczyść najmniej znaczący bit
        }

        // Ewaluacja skoczków
        while (knights) {
            int square = __builtin_ctzll(knights); // Znajdź indeks pierwszego ustawionego bitu
            evaluation += 300 + EvaluateBoard::knightTable[square]; // Przykładowa wartość punktowa dla skoczka
            knights &= knights - 1; // Wyczyść najmniej znaczący bit
        }

        while (bishops) {
            int square = __builtin_ctzll(bishops); // Znajdź indeks pierwszego ustawionego bitu
            evaluation += 300 + EvaluateBoard::bishopTable[square]; // Przykładowa wartość punktowa dla gońca
            bishops &= bishops - 1; // Wyczyść najmniej znaczący bit
        }

        // Ewaluacja wież
        while (rooks) {
            int square = __builtin_ctzll(rooks); // Znajdź indeks pierwszego ustawionego bitu
            evaluation += 500 + EvaluateBoard::rookTable[square]; // Przykładowa wartość punktowa dla wieży
            rooks &= rooks - 1; // Wyczyść najmniej znaczący bit
        }

        // Ewaluacja hetmanów
        while (queens) {
            int square = __builtin_ctzll(queens); // Znajdź indeks pierwszego ustawionego bitu
            evaluation += 900 + EvaluateBoard::queenTable[square]; // Przykładowa wartość punktowa dla hetmana
            queens &= queens - 1; // Wyczyść najmniej znaczący bit
        }

        // Ewaluacja króla
        while (kings) {
            int square = __builtin_ctzll(kings); // Znajdź indeks pierwszego ustawionego bitu
            evaluation += 10000 + EvaluateBoard::kingTable[square]; // Przykładowa wartość punktowa dla króla
            kings &= kings - 1; // Wyczyść najmniej znaczący bit
        }

        return evaluation;
    }

    friend class Engine;
};


int EvaluateBoard::pawnTable[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        5, 5, 5, -10, -10, 5, 5, 5,
        5, -5, -5, 0, 0, -5, -5, 5,
        0, 0, 0, 15, 15, 0, 0, 0,
        5, 5, 10, 20, 20, 10, 5, 5,
        10, 10, 35, 30, 35, 20, 10, 10,
        20, 20, 40, 45, 45, 40, 20, 20,
        0, 0, 0, 0, 0, 0, 0, 0
};

int EvaluateBoard::knightTable[64] = {
        -50, -40, -30, -30, -30, -30, -40, -50,
        -40, -20, 0, 5, 5, 0, -20, -40,
        -30, 5, 10, 15, 15, 10, 5, -30,
        -30, 0, 15, 20, 20, 15, 0, -30,
        -30, 5, 15, 20, 20, 15, 5, -30,
        -30, 0, 10, 15, 15, 10, 0, -30,
        -40, -20, 0, 0, 0, 0, -20, -40,
        -50, -40, -30, -30, -30, -30, -40, -50
};

int EvaluateBoard::bishopTable[64] = {
        -20, -10, -10, -10, -10, -10, -10, -20,
        -10, 0, 0, 0, 0, 0, 0, -10,
        -10, 0, 5, 10, 10, 5, 0, -10,
        -10, 5, 5, 10, 10, 5, 5, -10,
        -10, 0, 10, 10, 10, 10, 0, -10,
        -10, 10, 10, 10, 10, 10, 10, -10,
        -10, 5, 0, 0, 0, 0, 5, -10,
        -20, -10, -10, -10, -10, -10, -10, -20
};

int EvaluateBoard::rookTable[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        5, 10, 10, 10, 10, 10, 10, 5,
        -5, 0, 0, 0, 0, 0, 0, -5,
        -5, 0, 0, 0, 0, 0, 0, -5,
        -5, 0, 0, 0, 0, 0, 0, -5,
        -5, 0, 0, 0, 0, 0, 0, -5,
        -5, 0, 0, 0, 0, 0, 0, -5,
        0, 0, 0, 5, 5, 0, 0, 0
};

int EvaluateBoard::queenTable[64] = {
        -20, -10, -10, -5, -5, -10, -10, -20,
        -10, 0, 0, 0, 0, 0, 0, -10,
        -10, 0, 5, 5, 5, 5, 0, -10,
        -5, 0, 5, 5, 5, 5, 0, -5,
        0, 0, 5, 5, 5, 5, 0, -5,
        -10, 5, 5, 5, 5, 5, 0, -10,
        -10, 0, 5, 0, 0, 0, 0, -10,
        -20, -10, -10, -5, -5, -10, -10, -20
};

int EvaluateBoard::kingTable[64] = {
        -30, -40, -40, -50, -50, -40, -40, -30,
        -30, -40, -40, -50, -50, -40, -40, -30,
        -30, -40, -40, -50, -50, -40, -40, -30,
        -30, -40, -40, -50, -50, -40, -40, -30,
        -20, -30, -30, -40, -40, -30, -30, -20,
        -10, -20, -20, -20, -20, -20, -20, -10,
        20, 20, 0, 0, 0, 0, 20, 20,
        20, 30, 10, 0, 0, 10, 30, 20
};