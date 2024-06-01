#pragma once

#include <iostream>


typedef uint64_t Bitboard;

class BitBoard {
public:
    static void dump(const Bitboard& bitboard, std::string title = "") {

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
        bitboard = bitboard & ~(1ULL << from);
        bitboard |= (1ULL << to);

        return bitboard;
    }

};