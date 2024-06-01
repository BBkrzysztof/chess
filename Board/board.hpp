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

    void capture(int oldX, int oldY) {

        auto result = std::find_if(
                this->pieces.begin(),
                this->pieces.end(),
                [oldX, oldY](const std::pair<std::string, Piece*>& element) {
                    return element.second->getPositionX() == oldX * 100 && element.second->getPositionY() == oldY * 100;
                }
        );
        Piece* selectedPiece = result->second;

        Bitboard current = this->gameState->getBitBoard(
                selectedPiece->getPieceType(),
                selectedPiece->getPieceColor()
        );

        auto element = this->pieces.find(selectedPiece->getHash());
        this->pieces.erase(element);

        Bitboard newBitBoard = BitBoard::capture(current, (oldY * 8 + oldX));

        this->gameState->updateBitBoard(
                selectedPiece->getPieceType(),
                selectedPiece->getPieceColor(),
                newBitBoard
        );

        delete selectedPiece;
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

    void drawValidMoves(const Bitboard& validMoves, const Bitboard& captureMoves) {
        for (int rank = 7; rank >= 0; --rank) {
            for (int file = 0; file < 8; ++file) {
                if (validMoves & (1ULL << (rank * 8 + file))) {
                    auto* mi = new MoveIndicator(file, rank);
                    this->indicators.push_back(mi);
                }
            }
        }
        std::vector<MoveIndicator*> beatables;
        for (int rank = 7; rank >= 0; --rank) {
            for (int file = 0; file < 8; ++file) {
                if (captureMoves & (1ULL << (rank * 8 + file))) {
                    auto* mi = new MoveIndicator(file, rank, MoveOptions::Capture);
                    this->indicators.push_back(mi);
                    beatables.push_back(mi);
                }
            }
        }

        this->resetBeatable();

        for (const auto& beatable: beatables) {
            for (const auto& element: this->pieces) {
                if (element.second->getPositionX() == beatable->getPositionX() * 100 &&
                    element.second->getPositionY() == beatable->getPositionY() * 100) {
                    element.second->selectBeatable();
                }
            }
        }

    }

    void resetBeatable() {
        for (const auto& element: this->pieces) {
            element.second->unSelectBeatable();
        }
    }

private:

    static std::string pieceKey(Piece* piece) {
        static int id = 0;
        std::stringstream s;
        s << piece->getPieceType() << "-" << piece->getPieceColor() << "-" << id;
        piece->setHash(s.str());
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