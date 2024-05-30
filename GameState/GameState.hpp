#pragma once

#include "../Board/board.hpp"

typedef uint64_t Bitboard;

class GameState {

public:
    Bitboard calcOccupied() const {
        return whitePawns | blackPawns | whiteKnights | blackKnights |
               whiteBishops | blackBishops | whiteRooks | blackRooks | whiteQueen |
               blackQueen | whiteKing | blackKing;
    }


public:
    Bitboard whitePawns = 0x00FF000000000000ULL;
    Bitboard blackPawns = 0x000000000000FF00ULL;

    Bitboard whiteKnights = 0x4200000000000000ULL;
    Bitboard blackKnights = 0x0000000000000042ULL;

    Bitboard whiteBishops = 0x2400000000000000ULL;
    Bitboard blackBishops = 0x0000000000000024ULL;

    Bitboard whiteRooks = 0x8100000000000000ULL;
    Bitboard blackRooks = 0x0000000000000081ULL;

    Bitboard whiteQueen = 0x0800000000000000ULL;
    Bitboard blackQueen = 0x0000000000000008ULL;

    Bitboard whiteKing = 0x1000000000000000ULL;
    Bitboard blackKing = 0x0000000000000010ULL;

    friend class board;
};