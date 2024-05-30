#pragma once

#include <map>
#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>

#include "../Piece/King.hpp"
#include "../Piece/Pawn.hpp"
#include "../Piece/Rook.hpp"
#include "../Piece/Bishop.hpp"
#include "../Piece/Knight.hpp"
#include "../Piece/Queen.hpp"
#include "../Piece/Base/Piece.hpp"

typedef uint64_t Bitboard;

const sf::Color lightColor(240, 217, 181);
const sf::Color darkColor(181, 136, 99);
const int tileSize = 100;

class Board {
public:
    Board() {
        this->registerBlackPawns();
        this->registerWhitePawns();
        this->registerRooks();
        this->registerKnights();
        this->registerBishops();
        this->registerQueens();
        this->registerKings();
    };


    void draw(sf::RenderTarget& target) {
        // draw board
        for (int row = 0; row < 8; ++row) {
            for (int col = 0; col < 8; ++col) {
                sf::RectangleShape tile(sf::Vector2f(tileSize, tileSize));
                tile.setPosition(col * tileSize, row * tileSize);

                if ((row + col) % 2 == 0) {
                    tile.setFillColor(lightColor);
                } else {
                    tile.setFillColor(darkColor);
                }

                target.draw(tile);
            }
        }

        // draw pieces

        for (const auto& element: this->pieces) {

            if (element.first == this->selectedPiece) {
                element.second->selectPiece();
            } else {
                element.second->unSelectPiece();
            }

            element.second->draw(target);
        }
    }

    std::map<std::string, Piece*> getPieces() {
        return this->pieces;
    }

    void setSelectedPiece(std::string pieceHash) {
        this->selectedPiece = pieceHash;
    }

    std::string getSelectedPiece() {
        return this->selectedPiece;
    }

    Piece* getSelectedPieceReference() {
        return this->pieces[this->selectedPiece];
    }

private:

    static std::string pieceKey(Piece* piece) {
        static int id = 0;
        std::stringstream s;
        s << piece->getPieceType() << "-" << piece->getPieceColor() << "-" << id;
        id += 1;
        return s.str();
    }

    void registerBlackPawns() {
        auto* p1 = new Pawn(PieceColor::BLACK_PIECE, 0, tileSize);
        auto* p2 = new Pawn(PieceColor::BLACK_PIECE, 100, tileSize);
        auto* p3 = new Pawn(PieceColor::BLACK_PIECE, 200, tileSize);
        auto* p4 = new Pawn(PieceColor::BLACK_PIECE, 300, tileSize);
        auto* p5 = new Pawn(PieceColor::BLACK_PIECE, 400, tileSize);
        auto* p6 = new Pawn(PieceColor::BLACK_PIECE, 500, tileSize);
        auto* p7 = new Pawn(PieceColor::BLACK_PIECE, 600, tileSize);
        auto* p8 = new Pawn(PieceColor::BLACK_PIECE, 700, tileSize);

        this->registerPiece(p1);
        this->registerPiece(p2);
        this->registerPiece(p3);
        this->registerPiece(p4);
        this->registerPiece(p5);
        this->registerPiece(p6);
        this->registerPiece(p7);
        this->registerPiece(p8);
    }

    void registerWhitePawns() {
        auto* p1 = new Pawn(PieceColor::WHITE_PIECE, 0, 600);
        auto* p2 = new Pawn(PieceColor::WHITE_PIECE, 100, 600);
        auto* p3 = new Pawn(PieceColor::WHITE_PIECE, 200, 600);
        auto* p4 = new Pawn(PieceColor::WHITE_PIECE, 300, 600);
        auto* p5 = new Pawn(PieceColor::WHITE_PIECE, 400, 600);
        auto* p6 = new Pawn(PieceColor::WHITE_PIECE, 500, 600);
        auto* p7 = new Pawn(PieceColor::WHITE_PIECE, 600, 600);
        auto* p8 = new Pawn(PieceColor::WHITE_PIECE, 700, 600);

        this->registerPiece(p1);
        this->registerPiece(p2);
        this->registerPiece(p3);
        this->registerPiece(p4);
        this->registerPiece(p5);
        this->registerPiece(p6);
        this->registerPiece(p7);
        this->registerPiece(p8);
    }

    void registerRooks() {
        auto* p1 = new Rook(PieceColor::BLACK_PIECE, 0, 0);
        auto* p2 = new Rook(PieceColor::BLACK_PIECE, 700, 0);
        auto* p3 = new Rook(PieceColor::WHITE_PIECE, 0, 700);
        auto* p4 = new Rook(PieceColor::WHITE_PIECE, 700, 700);

        this->registerPiece(p1);
        this->registerPiece(p2);
        this->registerPiece(p3);
        this->registerPiece(p4);
    }

    void registerKnights() {
        auto* p1 = new Knight(PieceColor::BLACK_PIECE, 100, 0);
        auto* p2 = new Knight(PieceColor::BLACK_PIECE, 600, 0);
        auto* p3 = new Knight(PieceColor::WHITE_PIECE, 100, 700);
        auto* p4 = new Knight(PieceColor::WHITE_PIECE, 600, 700);

        this->registerPiece(p1);
        this->registerPiece(p2);
        this->registerPiece(p3);
        this->registerPiece(p4);
    }

    void registerBishops() {
        auto* p1 = new Bishop(PieceColor::BLACK_PIECE, 200, 0);
        auto* p2 = new Bishop(PieceColor::BLACK_PIECE, 500, 0);
        auto* p3 = new Bishop(PieceColor::WHITE_PIECE, 200, 700);
        auto* p4 = new Bishop(PieceColor::WHITE_PIECE, 500, 700);

        this->registerPiece(p1);
        this->registerPiece(p2);
        this->registerPiece(p3);
        this->registerPiece(p4);
    }

    void registerQueens() {
        auto* p1 = new Queen(PieceColor::BLACK_PIECE, 300, 0);
        auto* p2 = new Queen(PieceColor::WHITE_PIECE, 300, 700);

        this->registerPiece(p1);
        this->registerPiece(p2);
    }

    void registerKings() {
        auto* p1 = new King(PieceColor::BLACK_PIECE, 400, 0);
        auto* p2 = new King(PieceColor::WHITE_PIECE, 400, 700);

        this->registerPiece(p1);
        this->registerPiece(p2);
    }

    void registerPiece(Piece* piece) {
        this->pieces[Board::pieceKey(piece)] = piece;
    }


private:
    std::map<std::string, Piece*> pieces;
    std::string selectedPiece;

    bool isCheck;
    bool isCheckmate;
    bool isStalemate;

};