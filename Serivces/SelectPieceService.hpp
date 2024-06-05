#pragma once

#include "../Board/board.h"
#include "../Engine/MoveSimulator.hpp"

class SelectPieceService {
public:
    static void Select(Piece* piece, Board* board) {
        SelectPieceService* service = new SelectPieceService(board, board->gameState);
        service->selectPiece(piece);
    }

private:

    SelectPieceService(Board* board, GameState* gameState) {
        this->board = board;
        this->gameState = gameState;
    }

    void selectPiece(Piece* piece) {
        this->board->setSelectedPiece(piece->getHash());
        this->gameState->kingCastleMove = 0ULL;
        this->gameState->queenCastleMove = 0ULL;

        auto selectedPiece = this->board->getSelectedPieceReference();
        selectedPiece->rebuildValidMoves(
                this->gameState->calcBeatable(selectedPiece->getPieceColor()),
                this->gameState->calcOccupied(),
                this->gameState->peekHistory()
        );

        if (selectedPiece->getPieceType() == PieceType::PAWN) {
            Pawn::getValidMoves(
                    BitBoard::calcShift(selectedPiece->getPositionX() / 100, selectedPiece->getPositionY() / 100),
                    this->gameState->calcBeatable(selectedPiece->getPieceColor()),
                    this->gameState->calcOccupied(),
                    selectedPiece->getPieceColor(),
                    [this](Bitboard bitboard) {
                        this->isValidEnPassantMove(this->gameState->peekHistory(), bitboard);
                    }
            );
        }

        Bitboard validMoves = selectedPiece->getValidMoves() & ~this->gameState->calcOccupied();

        this->isMovesKingIntoCheck(selectedPiece, validMoves);
        this->checkCastle(selectedPiece);

        Bitboard captureMoves = selectedPiece->getValidMoves() & this->gameState->calcBeatable(
                selectedPiece->getPieceColor()
        );

        this->checkPromotion(selectedPiece, captureMoves);

        this->gameState->captureMoves = captureMoves = (captureMoves | this->gameState->getEnPassantMove()) &
                                                       ~this->gameState->captureAndPromotionMove;

        this->gameState->validMoves = validMoves &= (~this->gameState->promotionMove);

        this->board->drawValidMoves(
                this->gameState->validMoves,
                this->gameState->captureMoves,
                this->gameState->kingCastleMove,
                this->gameState->queenCastleMove,
                this->gameState->promotionMove,
                this->gameState->captureAndPromotionMove
        );

        this->validateNotOpeningCheck();
        this->ValidateCheckEscape();

        this->gameState->setEnPassantMove(0LL);
        this->gameState->kingCastleMove = 0ULL;
        this->gameState->queenCastleMove = 0ULL;
        this->gameState->validMoves = 0ULL;
        this->gameState->captureMoves = 0ULL;
    }

    void checkPromotion(Piece* selectedPiece, Bitboard captureMoves) {
        this->gameState->promotionMove = 0ULL;
        this->gameState->captureAndPromotionMove = 0ULL;

        if (selectedPiece->getPieceType() != PieceType::PAWN) {
            return;
        }

        Bitboard ROW_0 = 0x00000000000000FFULL;
        Bitboard ROW_7 = 0xFF00000000000000ULL;

        Bitboard validMoves = selectedPiece->getValidMoves() & ~this->gameState->calcOccupied();

        if (selectedPiece->getPieceColor() == PieceColor::WHITE_PIECE) {
            this->gameState->promotionMove = (ROW_0 & validMoves);
            this->gameState->captureAndPromotionMove = (ROW_0 & captureMoves);
            return;
        }

        this->gameState->promotionMove = (ROW_7 & validMoves);
        this->gameState->captureAndPromotionMove = (ROW_7 & captureMoves);
    }

    void checkCastle(Piece* selectedPiece) {
        if (selectedPiece->getPieceType() != PieceType::KING) {
            this->gameState->kingCastleMove = 0ULL;
            this->gameState->queenCastleMove = 0ULL;
            return;
        }

        int i = BitBoard::calcShift(selectedPiece->getPositionX() / 100, selectedPiece->getPositionY() / 100);
        Bitboard castle = 1ULL << i;

        bool isWhite = selectedPiece->getPieceColor() == PieceColor::WHITE_PIECE;

        bool kingSideCastle = this->gameState->canCastle(isWhite, true);
        bool queenSideCastle = this->gameState->canCastle(isWhite, false);

        if (kingSideCastle) {
            this->gameState->kingCastleMove = (castle << 2);
        }

        if (queenSideCastle) {
            this->gameState->queenCastleMove = (castle >> 2);
        }

    }

    void isMovesKingIntoCheck(Piece* selectedPiece, Bitboard& oldValidMoves) {

        if (selectedPiece->getPieceType() != PieceType::KING) {
            return;
        }

        auto enemyColor = selectedPiece->getPieceColor() == PieceColor::WHITE_PIECE ? PieceColor::BLACK_PIECE
                                                                                    : PieceColor::WHITE_PIECE;

        Bitboard attacked = this->gameState->getAttackedSquares(enemyColor);

        oldValidMoves = oldValidMoves & (~attacked);
    }

    void isValidEnPassantMove(MoveHistoryElement* lastMove, Bitboard current) {

        if (lastMove == nullptr) {
            return;
        }

        int shift = abs(lastMove->to - lastMove->from);

        if (lastMove->type != PieceType::PAWN || (shift != 16)) {
            return;
        }

        Bitboard pawns = this->gameState->getBitBoard(lastMove->type, lastMove->color);

        Bitboard reverse = BitBoard::moveOnBitBoard(pawns, lastMove->to, lastMove->from);

        if (reverse & current) {
            this->gameState->setEnPassantMove(current);
        } else {
            this->gameState->setEnPassantMove(0LL);
        }
    }

    void validateNotOpeningCheck() {
        std::vector<MoveIndicator*> validIndicators;
        GameState* gameStateEntryCopy = new GameState(*this->gameState);
        Board* boardEntryCopy = new Board(*this->board, gameStateEntryCopy, this->board->getIndicators());

        for (const auto& indicator: boardEntryCopy->getIndicators()) {
            bool isCheck = MoveSimulator::simulateMoveAndCheckIsCheck(
                    indicator,
                    gameStateEntryCopy,
                    boardEntryCopy
            );

            if (!isCheck) {
                validIndicators.push_back(indicator);
            }
        }

        delete boardEntryCopy;

        this->board->setIndicators(validIndicators);
    }

    void ValidateCheckEscape() {
        if (!this->gameState->getIsCheck()) {
            return;
        }

        std::vector<MoveIndicator*> validIndicators;
        GameState* gameStateEntryCopy = new GameState(*this->gameState);
        Board* boardEntryCopy = new Board(*this->board, gameStateEntryCopy, this->board->getIndicators());

        for (const auto& indicator: boardEntryCopy->getIndicators()) {
            bool isCheck = MoveSimulator::simulateMoveAndCheckIsCheck(
                    indicator,
                    gameStateEntryCopy,
                    boardEntryCopy
            );

            if (!isCheck) {
                validIndicators.push_back(indicator);
            }
        }

        delete boardEntryCopy;

        this->board->setIndicators(validIndicators);
    }

private:
    Board* board;
    GameState* gameState;
};