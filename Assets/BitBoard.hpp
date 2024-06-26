#pragma once

#include <iostream>


typedef uint64_t Bitboard;
const Bitboard FULL_BIT_BOARD = 0xFFFFFFFFFFFFFFFFULL;

enum PieceColor {
    WHITE_PIECE = 0,
    BLACK_PIECE = 1
};

enum PieceType {
    PAWN = 0,
    KNIGHT = 1,
    BISHOP = 2,
    ROOK = 3,
    QUEEN = 4,
    KING = 5,
};


class BitBoard {
public:

    static void dump(const Bitboard& bitboard, const std::string& title = "") {

        if (!title.empty()) {
            std::cout << "\n\n\n\n ======== " << title << " ======== \n\n\n\n";
        }

        for (int rank = 7; rank >= 0; --rank) {
            for (int file = 0; file < 8; ++file) {
                if (bitboard & (1ULL << (rank * 8 + file))) {
                    std::cout << "1";
                } else {
                    std::cout << "0";
                }
            }
            std::cout << std::endl;
        }
    }

    static Bitboard moveOnBitBoard(Bitboard bitboard, int from, int to) {
        bitboard = BitBoard::capture(bitboard, from);
        bitboard = BitBoard::insert(bitboard, to);

        return bitboard;
    }

    static Bitboard insert(Bitboard bitboard, int to) {
        bitboard |= (1ULL << to);

        return bitboard;
    }

    static Bitboard capture(Bitboard bitboard, int from) {
        bitboard &= ~(1ULL << from);
        return bitboard;
    }

    static bool isSquareAttacked(const Bitboard& square, const Bitboard& attacked) {
        return (attacked & square) != 0;
    }

    static int calcShift(int x, int y) {
        if (x > 100) {
            x /= 100;
        }
        if (y > 100) {
            y /= 100;
        }

        return y * 8 + x;
    }

    static bool matchBitboard(const Bitboard& bitboard, int shift) {
        Bitboard d = 0LL << shift;

        return (bitboard & (d));
    }
};