#pragma once

#include <map>
#include <unordered_map>
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

const std::string WHITE_KING = "white_king";
const std::string BLACK_KING = "black_king";

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

        // draw indicators

        for (const auto& element: this->indicators) {
            element->draw(target);
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

    void move(Piece* selectedPiece, int newX, int newY, bool captured = false) {
        int oldX = selectedPiece->getPositionX() / 100;
        int oldY = selectedPiece->getPositionY() / 100;

        Bitboard current = this->gameState->getBitBoard(
                selectedPiece->getPieceType(),
                selectedPiece->getPieceColor()
        );

        Bitboard newBitBoard = BitBoard::moveOnBitBoard(
                current,
                BitBoard::calcShift(oldX, oldY),
                BitBoard::calcShift(newX, newY)
        );

        selectedPiece->move(newX, newY);

        this->gameState->updateBitBoard(
                selectedPiece->getPieceType(),
                selectedPiece->getPieceColor(),
                newBitBoard
        );


        this->gameState->updateHistory(
                BitBoard::calcShift(oldX, oldY),
                BitBoard::calcShift(newX, newY),
                captured ? MoveOptions::Capture : MoveOptions::Move,
                selectedPiece
        );

        this->gameState->toggleTurn();
        this->setSelectedPiece("");
    }

    void castle(MoveOptions options, PieceColor pieceColor) {
        int oldX = 0, oldY = 0;

        if (options == MoveOptions::KING_SIDE_CASTLE) {
            if (pieceColor == PieceColor::WHITE_PIECE) {
                oldX = 7;
                oldY = 7;
            } else {
                oldX = 7;
                oldY = 0;
            }
        } else {
            if (pieceColor == PieceColor::WHITE_PIECE) {
                oldX = 0;
                oldY = 7;
            } else {
                oldX = 0;
                oldY = 0;
            }
        }

        auto result = std::find_if(
                this->pieces.begin(),
                this->pieces.end(),
                [oldX, oldY](const std::pair<std::string, Piece*>& element) {
                    return element.second->getPositionX() == oldX * 100 && element.second->getPositionY() == oldY * 100;
                }
        );

        Piece* selectedPiece = result->second;
        Piece* king = this->pieces[pieceColor == PieceColor::WHITE_PIECE ? WHITE_KING : BLACK_KING];

        Bitboard rookBoard = this->gameState->getBitBoard(
                selectedPiece->getPieceType(),
                selectedPiece->getPieceColor()
        );

        Bitboard kingBoard = this->gameState->getBitBoard(
                king->getPieceType(),
                king->getPieceColor()
        );

        int rookX = 0;
        int kingX = 0;
        int rookY = selectedPiece->getPositionY() / 100;
        int kingY = king->getPositionY() / 100;

        if (options == MoveOptions::KING_SIDE_CASTLE) {
            rookX = selectedPiece->getPositionX() / 100 - 2;
            kingX = king->getPositionX() / 100 + 2;
        } else {
            rookX = selectedPiece->getPositionX() / 100 + 3;
            kingX = king->getPositionX() / 100 - 2;
        }

        Bitboard newRookBitBoard = BitBoard::moveOnBitBoard(
                rookBoard,
                BitBoard::calcShift(selectedPiece->getPositionX() / 100, rookY),
                BitBoard::calcShift(rookX, rookY)
        );

        Bitboard newKingBitBoard = BitBoard::moveOnBitBoard(
                kingBoard,
                BitBoard::calcShift(king->getPositionX() / 100, kingY),
                BitBoard::calcShift(kingX, kingY)
        );

        selectedPiece->move(rookX, rookY);
        king->move(kingX, kingY);

        this->gameState->updateBitBoard(
                selectedPiece->getPieceType(),
                selectedPiece->getPieceColor(),
                newRookBitBoard
        );

        this->gameState->updateBitBoard(
                king->getPieceType(),
                king->getPieceColor(),
                newKingBitBoard
        );

        if (pieceColor == PieceColor::WHITE_PIECE) {
            this->gameState->whiteCastleMoved = true;
        } else {
            this->gameState->blackCastleMoved = true;
        }

        this->gameState->updateHistory(
                0,
                0,
                options,
                king
        );

        this->gameState->toggleTurn();
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
        Piece* selectedPiece;

        if (result != this->pieces.end()) {
            selectedPiece = result->second;
        } else {
            selectedPiece = this->gameState->peekHistory()->piece;
        }

        Bitboard current = this->gameState->getBitBoard(
                selectedPiece->getPieceType(),
                selectedPiece->getPieceColor()
        );

        this->gameState->markFirstMove(selectedPiece);

        auto element = this->pieces.find(selectedPiece->getHash());
        this->pieces.erase(element);

        Bitboard newBitBoard = BitBoard::capture(current, BitBoard::calcShift(oldX, oldY));

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

    void drawValidMoves(
            const Bitboard& validMoves,
            const Bitboard& captureMoves,
            const Bitboard& kingSideCastle,
            const Bitboard& queenSideCastle
    ) {
        for (int rank = 7; rank >= 0; --rank) {
            for (int file = 0; file < 8; ++file) {
                if (validMoves & (1ULL << BitBoard::calcShift(file, rank))) {
                    auto* mi = new MoveIndicator(file, rank);
                    this->indicators.push_back(mi);
                }
            }
        }

        for (int rank = 7; rank >= 0; --rank) {
            for (int file = 0; file < 8; ++file) {
                if (captureMoves & (1ULL << BitBoard::calcShift(file, rank))) {
                    auto* mi = new MoveIndicator(file, rank, MoveOptions::Capture);
                    this->indicators.push_back(mi);
                }
            }
        }

        for (int rank = 7; rank >= 0; --rank) {
            for (int file = 0; file < 8; ++file) {
                if (kingSideCastle & (1ULL << BitBoard::calcShift(file, rank))) {
                    auto* mi = new MoveIndicator(file, rank, MoveOptions::KING_SIDE_CASTLE);
                    this->indicators.push_back(mi);
                }
                if (queenSideCastle & (1ULL << BitBoard::calcShift(file, rank))) {
                    auto* mi = new MoveIndicator(file, rank, MoveOptions::QUEEN_SIDE_CASTLE);
                    this->indicators.push_back(mi);
                }
            }
        }
    }

    void isCheck() {
        auto color = Container::getGameState()->getTurn();
        bool isEnemyWhite = color == PieceColor::WHITE_PIECE;

        Bitboard attacks = Container::getGameState()->getAttackedSquares(
                isEnemyWhite ? PieceColor::BLACK_PIECE : PieceColor::WHITE_PIECE);

        Bitboard kingPosition = Container::getGameState()->getBitBoard(PieceType::KING, color);

        if ((kingPosition & attacks) != 0LL) {
            Container::getGameState()->setIsCheck(true);
            std::cout << "szach" << std::endl;
        } else {
            Container::getGameState()->setIsCheck(false);
        }
    }

    void isStalemate() {

    }

    void isCheckMate() {

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
            this->gameState->teams[PieceColor::WHITE_PIECE].push_back(piece);
        }, this->gameState->getWhitePawns());

        Board::registerPiecesGroup([this](const int& x, const int& y) {
            auto* piece = new Pawn(PieceColor::BLACK_PIECE, x, y);
            this->registerPiece(piece);
            this->gameState->teams[PieceColor::BLACK_PIECE].push_back(piece);
        }, this->gameState->getBlackPawns());
    }

    void registerKnights() {
        Board::registerPiecesGroup([this](const int& x, const int& y) {
            auto* piece = new Knight(PieceColor::WHITE_PIECE, x, y);
            this->registerPiece(piece);
            this->gameState->teams[PieceColor::WHITE_PIECE].push_back(piece);
        }, this->gameState->getWhiteKnights());


        Board::registerPiecesGroup([this](const int& x, const int& y) {
            auto* piece = new Knight(PieceColor::BLACK_PIECE, x, y);
            this->registerPiece(piece);
            this->gameState->teams[PieceColor::BLACK_PIECE].push_back(piece);
        }, this->gameState->getBlackKnights());
    }

    void registerBishops() {
        Board::registerPiecesGroup([this](const int& x, const int& y) {
            auto* piece = new Bishop(PieceColor::WHITE_PIECE, x, y);
            this->registerPiece(piece);
            this->gameState->teams[PieceColor::WHITE_PIECE].push_back(piece);
        }, this->gameState->getWhiteBishops());


        Board::registerPiecesGroup([this](const int& x, const int& y) {
            auto* piece = new Bishop(PieceColor::BLACK_PIECE, x, y);
            this->registerPiece(piece);
            this->gameState->teams[PieceColor::BLACK_PIECE].push_back(piece);
        }, this->gameState->getBlackBishops());
    }

    void registerRooks() {
        Board::registerPiecesGroup([this](const int& x, const int& y) {
            auto* piece = new Rook(PieceColor::WHITE_PIECE, x, y);
            this->registerPiece(piece);
            this->gameState->teams[PieceColor::WHITE_PIECE].push_back(piece);
        }, this->gameState->getWhiteRooks());


        Board::registerPiecesGroup([this](const int& x, const int& y) {
            auto* piece = new Rook(PieceColor::BLACK_PIECE, x, y);
            this->registerPiece(piece);
            this->gameState->teams[PieceColor::BLACK_PIECE].push_back(piece);
        }, this->gameState->getBlackRooks());

    }

    void registerQueens() {
        Board::registerPiecesGroup([this](const int& x, const int& y) {
            auto* piece = new Queen(PieceColor::WHITE_PIECE, x, y);
            this->registerPiece(piece);
            this->gameState->teams[PieceColor::WHITE_PIECE].push_back(piece);
        }, this->gameState->getWhiteQueen());


        Board::registerPiecesGroup([this](const int& x, const int& y) {
            auto* piece = new Queen(PieceColor::BLACK_PIECE, x, y);
            this->registerPiece(piece);
            this->gameState->teams[PieceColor::BLACK_PIECE].push_back(piece);
        }, this->gameState->getBlackQueen());
    }

    void registerKings() {
        Board::registerPiecesGroup([this](const int& x, const int& y) {
            auto* piece = new King(PieceColor::WHITE_PIECE, x, y);
            this->registerPiece(piece, WHITE_KING);
            this->gameState->teams[PieceColor::WHITE_PIECE].push_back(piece);
        }, this->gameState->getWhiteKing());


        Board::registerPiecesGroup([this](const int& x, const int& y) {
            auto* piece = new King(PieceColor::BLACK_PIECE, x, y);
            this->registerPiece(piece, BLACK_KING);
            this->gameState->teams[PieceColor::BLACK_PIECE].push_back(piece);
        }, this->gameState->getBlackKing());
    }

    static void registerPiecesGroup(const std::function<void(int, int)>& reg, const Bitboard& bitboard) {
        for (int rank = 7; rank >= 0; --rank) {
            for (int file = 0; file < 8; ++file) {
                if (bitboard & (1ULL << BitBoard::calcShift(file, rank))) {
                    reg(file, rank);
                }
            }
        }
    }

    void registerPiece(Piece* piece, std::string key = "") {
        auto pieceKey = key.empty() ? Board::pieceKey(piece) : key;
        this->pieces[pieceKey] = piece;
    }

private:
    std::map<std::string, Piece*> pieces;

    std::vector<MoveIndicator*> indicators;

    std::string selectedPiece;

    GameState* gameState;

    friend class GameState;
};