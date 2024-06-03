#pragma once

#include "board.h"

Board::Board(GameState* gameState) {
    this->gameState = gameState;

    this->registerPawns();
    this->registerKnights();
    this->registerBishops();
    this->registerRooks();
    this->registerQueens();
    this->registerKings();
}

Board::Board(const Board& board, GameState* gameStateCopy) {
    this->gameState = gameStateCopy;
    this->indicators = board.indicators;
    this->pieces = board.pieces;
    this->selectedPiece = board.selectedPiece;
}


void Board::draw(sf::RenderTarget& target) {
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

std::map<std::string, Piece*> Board::getPieces() {
    return this->pieces;
}

void Board::setSelectedPiece(const std::string& pieceHash) {
    this->selectedPiece = pieceHash;
    this->clearIndicators();
}

std::string Board::getSelectedPiece() {
    return this->selectedPiece;
}

Piece* Board::getSelectedPieceReference() {
    return this->pieces[this->selectedPiece];
}

void Board::clearIndicators() {
    this->indicators.clear();
}

std::vector<MoveIndicator*> Board::getIndicators() {
    return this->indicators;
}

void Board::drawValidMoves(
        const Bitboard& validMoves,
        const Bitboard& captureMoves,
        const Bitboard& kingSideCastle,
        const Bitboard& queenSideCastle,
        const Bitboard& promotions,
        const Bitboard& captureAndPromotion
) {
    for (int rank = 7; rank >= 0; --rank) {
        for (int file = 0; file < 8; ++file) {
            if (validMoves & (1ULL << BitBoard::calcShift(file, rank))) {
                auto* mi = new MoveIndicator(file, rank);
                this->indicators.push_back(mi);
            }
            if (captureMoves & (1ULL << BitBoard::calcShift(file, rank))) {
                auto* mi = new MoveIndicator(file, rank, MoveOptions::Capture);
                this->indicators.push_back(mi);
            }
            if (kingSideCastle & (1ULL << BitBoard::calcShift(file, rank))) {
                auto* mi = new MoveIndicator(file, rank, MoveOptions::KING_SIDE_CASTLE);
                this->indicators.push_back(mi);
            }
            if (queenSideCastle & (1ULL << BitBoard::calcShift(file, rank))) {
                auto* mi = new MoveIndicator(file, rank, MoveOptions::QUEEN_SIDE_CASTLE);
                this->indicators.push_back(mi);
            }
            if (promotions & (1ULL << BitBoard::calcShift(file, rank))) {
                auto* mi = new MoveIndicator(file, rank, MoveOptions::PROMOTION);
                this->indicators.push_back(mi);
            }
            if (captureAndPromotion & (1ULL << BitBoard::calcShift(file, rank))) {
                auto* mi = new MoveIndicator(file, rank, MoveOptions::CAPTURE_AND_PROMOTION);
                this->indicators.push_back(mi);
            }
        }
    }
}

void Board::isCheck() {
    auto color = Container::getGameState()->getTurn();
    bool isEnemyWhite = color == PieceColor::WHITE_PIECE;

    Bitboard attacks = this->gameState->getAttackedSquares(
            isEnemyWhite ? PieceColor::BLACK_PIECE : PieceColor::WHITE_PIECE);

    Bitboard kingPosition = Container::getGameState()->getBitBoard(PieceType::KING, color);

    if ((kingPosition & attacks) != 0LL) {
        Container::getGameState()->setIsCheck(true);
        std::cout << "szach" << std::endl;
    } else {
        Container::getGameState()->setIsCheck(false);
    }
}

void Board::registerPawns() {
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

void Board::registerKnights() {
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

void Board::registerBishops() {
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

void Board::registerRooks() {
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

void Board::registerQueens() {
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

void Board::registerKings() {
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

void Board::registerPiecesGroup(const std::function<void(int, int)>& reg, const Bitboard& bitboard) {
    for (int rank = 7; rank >= 0; --rank) {
        for (int file = 0; file < 8; ++file) {
            if (bitboard & (1ULL << BitBoard::calcShift(file, rank))) {
                reg(file, rank);
            }
        }
    }
}

void Board::registerPiece(Piece* piece, std::string key) {
    auto pieceKey = key.empty() ? Helper::getHash(piece) : key;
    this->pieces[pieceKey] = piece;
}

void Board::setPieces(std::map<std::string, Piece*> pieces) {
    this->pieces = pieces;
}
