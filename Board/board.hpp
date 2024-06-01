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
#include "../Piece/Base/MoveIndicator.hpp"

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

        for (const auto& element: this->indicators) {
            element->draw(target);
        }
    }

    std::map<std::string, Piece*> getPieces() {
        return this->pieces;
    }

    void move(Piece* selectedPiece, int newX, int newY) {
        int oldX = selectedPiece->getPositionX() / 100;
        int oldY = selectedPiece->getPositionY() / 100;

        Bitboard current = this->gameState->getBitBoard(
                selectedPiece->getPieceType(),
                selectedPiece->getPieceColor()
        );

        Bitboard newBitBoard = BitBoard::moveOnBitBoard(
                current,
                (oldY * 8 + oldX),
                (newY * 8 + newX)
        );

        selectedPiece->move(newX, newY);

        this->gameState->updateBitBoard(
                selectedPiece->getPieceType(),
                selectedPiece->getPieceColor(),
                newBitBoard
        );

        this->setSelectedPiece("");
    }

    void setSelectedPiece(const std::string& pieceHash) {
        this->selectedPiece = pieceHash;
        this->clearIndicators();
    }

    std::string getSelectedPiece() {
        return this->selectedPiece;
    }

    Piece* getSelectedPieceReference() {
        return this->pieces[this->selectedPiece];
    }

    void clearIndicators() {
        this->indicators.clear();
    }

    std::vector<MoveIndicator*> getIndicators() {
        return this->indicators;
    }

    void markBeatable(const Bitboard& bitboard) {
        auto pieces = this->getPieces();
        for (int rank = 7; rank >= 0; --rank) {
            for (int file = 0; file < 8; ++file) {
                if (bitboard & (1ULL << (rank * 8 + file))) {

                }
            }
        }
    }

    void drawValidMoves(const Bitboard& bitboard) {
        for (int rank = 7; rank >= 0; --rank) {
            for (int file = 0; file < 8; ++file) {
                if (bitboard & (1ULL << (rank * 8 + file))) {
                    auto* mi = new MoveIndicator(file, rank);
                    this->indicators.push_back(mi);
                }
            }
        }
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
        Board::registerPiecesGroup([this](const int& x, const int& y) {
            auto* piece = new Pawn(PieceColor::WHITE_PIECE, x, y);
            this->registerPiece(piece);
        }, this->gameState->getWhitePawns());

        Board::registerPiecesGroup([this](const int& x, const int& y) {
            auto* piece = new Pawn(PieceColor::BLACK_PIECE, x, y);
            this->registerPiece(piece);
        }, this->gameState->getBlackPawns());
    }

    void registerKnights() {
        Board::registerPiecesGroup([this](const int& x, const int& y) {
            auto* piece = new Knight(PieceColor::WHITE_PIECE, x, y);
            this->registerPiece(piece);
        }, this->gameState->getWhiteKnights());


        Board::registerPiecesGroup([this](const int& x, const int& y) {
            auto* piece = new Knight(PieceColor::BLACK_PIECE, x, y);
            this->registerPiece(piece);
        }, this->gameState->getBlackKnights());
    }

    void registerBishops() {
        Board::registerPiecesGroup([this](const int& x, const int& y) {
            auto* piece = new Bishop(PieceColor::WHITE_PIECE, x, y);
            this->registerPiece(piece);
        }, this->gameState->getWhiteBishops());


        Board::registerPiecesGroup([this](const int& x, const int& y) {
            auto* piece = new Bishop(PieceColor::BLACK_PIECE, x, y);
            this->registerPiece(piece);
        }, this->gameState->getBlackBishops());
    }

    void registerRooks() {
        Board::registerPiecesGroup([this](const int& x, const int& y) {
            auto* piece = new Rook(PieceColor::WHITE_PIECE, x, y);
            this->registerPiece(piece);
        }, this->gameState->getWhiteRooks());


        Board::registerPiecesGroup([this](const int& x, const int& y) {
            auto* piece = new Rook(PieceColor::BLACK_PIECE, x, y);
            this->registerPiece(piece);
        }, this->gameState->getBlackRooks());

    }

    void registerQueens() {
        Board::registerPiecesGroup([this](const int& x, const int& y) {
            auto* piece = new Queen(PieceColor::WHITE_PIECE, x, y);
            this->registerPiece(piece);
        }, this->gameState->getWhiteQueen());


        Board::registerPiecesGroup([this](const int& x, const int& y) {
            auto* piece = new Queen(PieceColor::BLACK_PIECE, x, y);
            this->registerPiece(piece);
        }, this->gameState->getBlackQueen());
    }

    void registerKings() {
        Board::registerPiecesGroup([this](const int& x, const int& y) {
            auto* piece = new King(PieceColor::WHITE_PIECE, x, y);
            this->registerPiece(piece);
        }, this->gameState->getWhiteKing());


        Board::registerPiecesGroup([this](const int& x, const int& y) {
            auto* piece = new King(PieceColor::BLACK_PIECE, x, y);
            this->registerPiece(piece);
        }, this->gameState->getBlackKing());
    }

    static void registerPiecesGroup(const std::function<void(int, int)>& reg, const Bitboard& bitboard) {
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
    std::vector<MoveIndicator*> indicators;

    std::string selectedPiece;

    GameState* gameState;

    bool isCheck;
    bool isCheckmate;
    bool isStalemate;

    friend class GameState;
};