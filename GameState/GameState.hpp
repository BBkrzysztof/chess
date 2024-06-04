#pragma once

#include <stack>
#include <algorithm>

#include "../Board/board.cpp"
#include "../Assets/BitBoard.hpp"
#include "../Piece/Base/MoveIndicator.hpp"
#include "../Assets/BitBoard.hpp"

#include "../Moves/MoveHistoryElement.hpp"

typedef uint64_t Bitboard;
typedef class MoveHistoryElement MoveElement;

class GameState {

public:
    GameState() = default;

    GameState(const GameState& gameState) {
        this->whiteKingMoved = gameState.whiteKingMoved;
        this->blackKingMoved = gameState.blackKingMoved;
        this->whiteRookA1Moved = gameState.whiteRookA1Moved;
        this->whiteRookH1Moved = gameState.whiteRookH1Moved;
        this->blackRookA8Moved = gameState.blackRookA8Moved;
        this->blackRookH8Moved = gameState.blackRookH8Moved;
        this->whiteCastleMoved = gameState.whiteCastleMoved;
        this->blackCastleMoved = gameState.blackCastleMoved;
        this->kingCastleMove = gameState.kingCastleMove;
        this->queenCastleMove = gameState.queenCastleMove;
        this->promotionMove = gameState.promotionMove;
        this->captureAndPromotionMove = gameState.captureAndPromotionMove;
        this->whitePawns = gameState.whitePawns;
        this->blackPawns = gameState.blackPawns;
        this->whiteKnights = gameState.whiteKnights;
        this->blackKnights = gameState.blackKnights;
        this->whiteBishops = gameState.whiteBishops;
        this->blackBishops = gameState.blackBishops;
        this->whiteRooks = gameState.whiteRooks;
        this->blackRooks = gameState.blackRooks;
        this->whiteQueen = gameState.whiteQueen;
        this->blackQueen = gameState.blackQueen;
        this->whiteKing = gameState.whiteKing;
        this->blackKing = gameState.blackKing;
        this->turn = gameState.turn;
        this->enPassantMove = gameState.enPassantMove;
        this->isCheck = gameState.isCheck;
        this->isCheckmate = gameState.isCheckmate;
        this->isStalemate = gameState.isStalemate;
        this->moveHistory = gameState.moveHistory;
        this->validMoves = gameState.validMoves;
        this->captureMoves = gameState.captureMoves;
    }

    Bitboard calcOccupied() const {
        return whitePawns | blackPawns | whiteKnights | blackKnights |
               whiteBishops | blackBishops | whiteRooks | blackRooks | whiteQueen |
               blackQueen | whiteKing | blackKing;
    }

    Bitboard calcBeatable(const PieceColor& color) const {
        if (color == PieceColor::WHITE_PIECE) {
            return blackPawns | blackKnights | blackBishops | blackRooks | blackQueen;
        }

        return whitePawns | whiteKnights | whiteBishops | whiteRooks | whiteQueen;
    }

    void updateHistory(
            int from,
            int to,
            MoveOptions options,
            Piece* piece
    ) {
        this->moveHistory.emplace(from, to, options, piece->getPieceColor(), piece->getPieceType(), piece);
    }

    void updateBitBoard(
            const PieceType& type,
            const PieceColor& color,
            const Bitboard& newValue
    ) {
        switch (type) {
            case PieceType::PAWN:
                color == PieceColor::WHITE_PIECE ? this->whitePawns = newValue : this->blackPawns = newValue;
                break;
            case PieceType::KNIGHT:
                color == PieceColor::WHITE_PIECE ? this->whiteKnights = newValue : this->blackKnights = newValue;
                break;
            case PieceType::BISHOP:
                color == PieceColor::WHITE_PIECE ? this->whiteBishops = newValue : this->blackBishops = newValue;
                break;
            case PieceType::ROOK:
                color == PieceColor::WHITE_PIECE ? this->whiteRooks = newValue : this->blackRooks = newValue;
                break;
            case PieceType::QUEEN:
                color == PieceColor::WHITE_PIECE ? this->whiteQueen = newValue : this->blackQueen = newValue;
                break;
            case PieceType::KING:
                color == PieceColor::WHITE_PIECE ? this->whiteKing = newValue : this->blackKing = newValue;
                break;
        }
    }

    Bitboard getBitBoard(const PieceType& type, const PieceColor& color) {
        switch (type) {
            case PieceType::PAWN:
                return color == PieceColor::WHITE_PIECE ? this->whitePawns : this->blackPawns;
                break;
            case PieceType::KNIGHT:
                return color == PieceColor::WHITE_PIECE ? this->whiteKnights : this->blackKnights;
                break;
            case PieceType::BISHOP:
                return color == PieceColor::WHITE_PIECE ? this->whiteBishops : this->blackBishops;
                break;
            case PieceType::ROOK:
                return color == PieceColor::WHITE_PIECE ? this->whiteRooks : this->blackRooks;
                break;
            case PieceType::QUEEN:
                return color == PieceColor::WHITE_PIECE ? this->whiteQueen : this->blackQueen;
                break;
            case PieceType::KING:
                return color == PieceColor::WHITE_PIECE ? this->whiteKing : this->blackKing;
                break;
        }
    }

    Bitboard getWhitePawns() {
        return this->whitePawns;
    }

    Bitboard getBlackPawns() {
        return this->blackPawns;
    }

    Bitboard getWhiteKnights() {
        return this->whiteKnights;
    }

    Bitboard getBlackKnights() {
        return this->blackKnights;
    }

    Bitboard getWhiteBishops() {
        return this->whiteBishops;
    }

    Bitboard getBlackBishops() {
        return this->blackBishops;
    }

    Bitboard getWhiteRooks() {
        return this->whiteRooks;
    }

    Bitboard getBlackRooks() {
        return this->blackRooks;
    }

    Bitboard getWhiteQueen() {
        return this->whiteQueen;
    }

    Bitboard getBlackQueen() {
        return this->blackQueen;
    }

    Bitboard getWhiteKing() {
        return this->whiteKing;
    }

    Bitboard getBlackKing() {
        return this->blackKing;
    }

    PieceColor getTurn() const {
        return this->turn;
    }

    void toggleTurn() {
        this->turn == PieceColor::WHITE_PIECE ? this->turn = PieceColor::BLACK_PIECE
                                              : this->turn = PieceColor::WHITE_PIECE;
    }

    bool getIsCheck() const {
        return this->isCheck;
    }

    bool getIsCheckMate() const {
        return this->isCheckmate;
    }

    bool getIsStalemate() const {
        return this->isStalemate;
    }

    void setIsCheck(bool value) {
        this->isCheck = value;
    }

    void setIsCheckMate(bool value) {
        this->isCheckmate = value;
    }

    void setIsStalemate(bool value) {
        this->isStalemate = value;
    }

    Bitboard getEnPassantMove() const {
        return this->enPassantMove;
    }

    void setEnPassantMove(Bitboard move) {
        this->enPassantMove = move;
    }

    MoveElement* peekHistory() {
        if (this->moveHistory.empty()) {
            return nullptr;
        }

        return &this->moveHistory.top();
    }

    Bitboard getAttackedSquares(PieceColor team) {

        std::vector<Piece*> enemyTeam = this->teams.at(team);

        Bitboard attacks = 0ULL;

        for (auto element: enemyTeam) {
            element->rebuildValidMoves(
                    this->calcBeatable(element->getPieceColor()),
                    this->calcOccupied(),
                    this->peekHistory()
            );
            attacks |= (element->getValidMoves());
        }

        return attacks;
    }

    bool canCastle(bool isWhite, bool kingSide) {
        Bitboard occupied = this->calcOccupied();

        if (!isWhite) {
            if (this->whiteKingMoved || this->whiteCastleMoved) return false;
            Bitboard attacked = this->getAttackedSquares(PieceColor::WHITE_PIECE);

            if (kingSide) {
                if (this->whiteRookH1Moved) return false;
                if (occupied & (0x60ULL)) return false;
                if (BitBoard::isSquareAttacked(0x60ULL, attacked)) return false;
            } else {
                if (this->whiteRookA1Moved) return false;
                if (occupied & (0xEULL)) return false;
                if (BitBoard::isSquareAttacked(0xEULL, attacked)) return false;
            }

        } else {
            Bitboard attacked = this->getAttackedSquares(PieceColor::BLACK_PIECE);

            if (this->blackKingMoved || this->blackCastleMoved) return false;
            if (kingSide) {
                if (this->blackRookH8Moved) return false;
                if (occupied & (0x6000000000000000ULL)) return false;
                if (BitBoard::isSquareAttacked(0x6000000000000000ULL, attacked)) return false;
            } else {
                if (this->blackRookA8Moved) return false;
                if (occupied & (0xE00000000000000ULL)) return false;
                if (BitBoard::isSquareAttacked(0xE00000000000000ULL, attacked)) return false;
            }
        }

        return true;
    }

    void markFirstMove(Piece* selectedPiece) {
        if (selectedPiece->getPieceType() == PieceType::KING) {
            if (selectedPiece->getPieceColor() == PieceColor::WHITE_PIECE) {
                this->whiteKingMoved = true;
            } else {
                this->blackKingMoved = true;
            }
        } else if (selectedPiece->getPieceType() == PieceType::ROOK) {
            int x = selectedPiece->getPositionX() / 100;
            int y = selectedPiece->getPositionY() / 100;

            if (x == 0 && y == 0) {
                this->blackRookA8Moved = true;
            } else if (x == 0 && y == 7) {
                this->blackRookH8Moved = true;
            } else if (x == 7 && y == 0) {
                this->whiteRookA1Moved = true;
            } else if (x == 7 && y == 7) {
                this->whiteRookH1Moved = true;
            }
        }
    }

    void removeFromTeam(Piece* selectedPiece) {
        auto color = selectedPiece->getPieceColor();
        auto targetVector = this->teams.at(color);
        auto selectedPieceHash = selectedPiece->getHash();

        auto result = std::find_if(
                targetVector.begin(),
                targetVector.end(),
                [selectedPieceHash](const Piece* element) {
                    return element->getHash() == selectedPieceHash;
                }
        );

        if (result == targetVector.end()) {
            throw std::exception();
        }

        targetVector.erase(result);
        this->teams.at(color) = targetVector;
    }

    void addToTeam(Piece* selectedPiece) {
        auto color = selectedPiece->getPieceColor();
        this->teams.at(color).push_back(selectedPiece);
    }

    std::vector<Piece*> getCurrentTeam() {
        return this->teams[this->turn];
    }

public:

    bool whiteKingMoved = false;
    bool blackKingMoved = false;
    bool whiteRookA1Moved = false;
    bool whiteRookH1Moved = false;
    bool blackRookA8Moved = false;
    bool blackRookH8Moved = false;
    bool whiteCastleMoved = false;
    bool blackCastleMoved = false;

    Bitboard kingCastleMove = 0ULL;
    Bitboard queenCastleMove = 0ULL;
    Bitboard promotionMove = 0ULL;
    Bitboard captureAndPromotionMove = 0ULL;

    Bitboard validMoves = 0ULL;
    Bitboard captureMoves = 0ULL;

    std::unordered_map<PieceColor, std::vector<Piece*>> teams;

private:
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

    PieceColor turn = PieceColor::WHITE_PIECE;

    Bitboard enPassantMove = 0LL;

    std::stack<MoveElement> moveHistory;


    bool isCheck;
    bool isCheckmate;
    bool isStalemate;

    friend class board;

};