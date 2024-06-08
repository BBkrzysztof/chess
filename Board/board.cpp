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

Board::Board(const Board& board, GameState* gameStateCopy, const std::vector<MoveIndicator*>& indicators) {
    this->gameState = gameStateCopy;

    for (const auto& piece: board.pieces) {
        this->pieces[piece.first] = PieceFactory::copy(piece.second);
    }

    this->selectedPiece = board.selectedPiece;
    this->indicators.clear();
    for (const auto& indicator: indicators) {
        this->indicators.push_back(new MoveIndicator(*indicator));
    }

    this->rebuildTeams();
}

Board::~Board() {
    for (const auto& piece: this->pieces) {
        delete piece.second;
    }
    this->pieces.clear();

    for (const auto& indicator: this->indicators) {
        delete indicator;
    }
    this->indicators.clear();
    this->gameState = nullptr;
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

Piece* Board::getSelectedPieceReference() const {
    return this->pieces.at(this->selectedPiece);
}

void Board::clearIndicators() {
    this->indicators.clear();
}

std::vector<MoveIndicator*> Board::getIndicators() {
    return this->indicators;
}

void Board::setIndicators(const std::vector<MoveIndicator*>& indicators) {
    for (const auto& indicator: this->indicators) {
        delete indicator;
    }
    this->indicators.clear();
    for (const auto& indicator: indicators) {
        this->indicators.push_back(new MoveIndicator(*indicator));
    }
}

void Board::drawValidMoves(
        const Bitboard& validMoves,
        const Bitboard& captureMoves,
        const Bitboard& kingSideCastle,
        const Bitboard& queenSideCastle,
        const Bitboard& promotions,
        const Bitboard& captureAndPromotion,
        bool lightMode
) {
    for (const auto& item: this->indicators) {
        delete item;
    }

    for (int rank = 7; rank >= 0; --rank) {
        for (int file = 0; file < 8; ++file) {
            if (validMoves & (1ULL << BitBoard::calcShift(file, rank))) {
                auto* mi = new MoveIndicator(file, rank, MoveOptions::Move, lightMode);
                this->indicators.push_back(mi);
            }
            if (captureMoves & (1ULL << BitBoard::calcShift(file, rank))) {
                auto* mi = new MoveIndicator(file, rank, MoveOptions::Capture, lightMode);
                this->indicators.push_back(mi);
            }
            if (kingSideCastle & (1ULL << BitBoard::calcShift(file, rank))) {
                auto* mi = new MoveIndicator(file, rank, MoveOptions::KING_SIDE_CASTLE, lightMode);
                this->indicators.push_back(mi);
            }
            if (queenSideCastle & (1ULL << BitBoard::calcShift(file, rank))) {
                auto* mi = new MoveIndicator(file, rank, MoveOptions::QUEEN_SIDE_CASTLE, lightMode);
                this->indicators.push_back(mi);
            }
            if (promotions & (1ULL << BitBoard::calcShift(file, rank))) {
                auto* mi = new MoveIndicator(file, rank, MoveOptions::PROMOTION, lightMode);
                this->indicators.push_back(mi);
            }
            if (captureAndPromotion & (1ULL << BitBoard::calcShift(file, rank))) {
                auto* mi = new MoveIndicator(file, rank, MoveOptions::CAPTURE_AND_PROMOTION, lightMode);
                this->indicators.push_back(mi);
            }
        }
    }
}

bool Board::isCheck(bool dump) {
    auto color = this->gameState->getTurn();
    bool isEnemyWhite = color == PieceColor::WHITE_PIECE;

    Bitboard attacks = this->gameState->getAttackedSquares(
            isEnemyWhite ? PieceColor::BLACK_PIECE : PieceColor::WHITE_PIECE);

    Bitboard kingPosition = this->gameState->getBitBoard(PieceType::KING, color);
    if (dump) {
        BitBoard::dump(kingPosition, "king");
        BitBoard::dump(attacks, "attack");
    }

    if ((kingPosition & attacks) != 0LL) {
        this->gameState->setIsCheck(true);
        return true;
    } else {
        this->gameState->setIsCheck(false);
        return false;
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

void Board::rebuildTeams() {
    this->gameState->teams.clear();
    for (const auto& piece: this->pieces) {
        this->gameState->teams[piece.second->getPieceColor()].push_back(piece.second);
    }
}

void Board::registerPiece(Piece* piece, std::string key) {
    auto pieceKey = key.empty() ? Helper::getHash(piece) : key;
    piece->setHash(pieceKey);
    this->pieces[pieceKey] = piece;
}

void Board::setPieces(std::map<std::string, Piece*> pieces) {
    this->pieces = pieces;
}

Piece* Board::getPieceByPosition(int x, int y) const {
    auto result = std::find_if(
            pieces.begin(),
            pieces.end(),
            [x, y](const std::pair<std::string, Piece*>& element) {
                return element.second->getPositionX() == x * 100 && element.second->getPositionY() == y * 100;
            }
    );

    if (result != this->pieces.end()) {
        return result->second;
    }

    return nullptr;
}
