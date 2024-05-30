#pragma once

#include <map>
#include <iostream>
#include <sstream>
#include <functional>
#include <SFML/Graphics.hpp>

#include "../GameState/GameState.hpp"

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
    Board(GameState* gameState) {
        this->gameState = gameState;

        this->registerPawns();
        this->registerKnights();
        this->registerBishops();
        this->registerRooks();
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

    void registerPawns() {
        this->registerPiecesGroup([this](const int& x, const int& y) {
            auto* piece = new Pawn(PieceColor::WHITE_PIECE, x, y);
            this->registerPiece(piece);
        }, this->gameState->whitePawns);

        this->registerPiecesGroup([this](const int& x, const int& y) {
            auto* piece = new Pawn(PieceColor::BLACK_PIECE, x, y);
            this->registerPiece(piece);
        }, this->gameState->blackPawns);
    }

    void registerKnights() {
        this->registerPiecesGroup([this](const int& x, const int& y) {
            auto* piece = new Knight(PieceColor::WHITE_PIECE, x, y);
            this->registerPiece(piece);
        }, this->gameState->whiteKnights);


        this->registerPiecesGroup([this](const int& x, const int& y) {
            auto* piece = new Knight(PieceColor::BLACK_PIECE, x, y);
            this->registerPiece(piece);
        }, this->gameState->blackKnights);
    }

    void registerBishops() {
        this->registerPiecesGroup([this](const int& x, const int& y) {
            auto* piece = new Bishop(PieceColor::WHITE_PIECE, x, y);
            this->registerPiece(piece);
        }, this->gameState->whiteBishops);


        this->registerPiecesGroup([this](const int& x, const int& y) {
            auto* piece = new Bishop(PieceColor::BLACK_PIECE, x, y);
            this->registerPiece(piece);
        }, this->gameState->blackBishops);
    }

    void registerRooks() {
        this->registerPiecesGroup([this](const int& x, const int& y) {
            auto* piece = new Rook(PieceColor::WHITE_PIECE, x, y);
            this->registerPiece(piece);
        }, this->gameState->whiteRooks);


        this->registerPiecesGroup([this](const int& x, const int& y) {
            auto* piece = new Rook(PieceColor::BLACK_PIECE, x, y);
            this->registerPiece(piece);
        }, this->gameState->blackRooks);

    }

    void registerQueens() {
        this->registerPiecesGroup([this](const int& x, const int& y) {
            auto* piece = new Queen(PieceColor::WHITE_PIECE, x, y);
            this->registerPiece(piece);
        }, this->gameState->whiteQueen);


        this->registerPiecesGroup([this](const int& x, const int& y) {
            auto* piece = new Queen(PieceColor::BLACK_PIECE, x, y);
            this->registerPiece(piece);
        }, this->gameState->blackQueen);
    }

    void registerKings() {
        this->registerPiecesGroup([this](const int& x, const int& y) {
            auto* piece = new King(PieceColor::WHITE_PIECE, x, y);
            this->registerPiece(piece);
        }, this->gameState->whiteKing);


        this->registerPiecesGroup([this](const int& x, const int& y) {
            auto* piece = new King(PieceColor::BLACK_PIECE, x, y);
            this->registerPiece(piece);
        }, this->gameState->blackKing);
    }

    void registerPiecesGroup(const std::function<void(int, int)>& reg, const Bitboard& bitboard) {
        for (int rank = 7; rank >= 0; --rank) {
            for (int file = 0; file < 8; ++file) {
                if (bitboard & (1ULL << (rank * 8 + file))) {
                    reg(file, rank);
                }
            }
        }
    }

    void registerPiece(Piece* piece) {
        this->pieces[Board::pieceKey(piece)] = piece;
    }


private:
    std::map<std::string, Piece*> pieces;
    std::string selectedPiece;

    GameState* gameState;

    bool isCheck;
    bool isCheckmate;
    bool isStalemate;

    friend class GameState;
};