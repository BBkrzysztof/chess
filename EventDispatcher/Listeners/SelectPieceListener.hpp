#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>

#include "../EventListenerInterface.hpp"
#include "../../Assets/BitBoard.hpp"

class SelectPieceListener : public EventListenerInterface {
    void onEvent(sf::Event event) final {

        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePosition = sf::Mouse::getPosition(*this->window);
                auto pieces = this->board->getPieces();

                for (const auto& element: pieces) {
                    if (element.second->getPieceColor() == this->gameState->getTurn() &&
                        element.second->validateBounds(mousePosition)) {

                        if (this->board->getSelectedPiece() == element.first) {
                            this->board->setSelectedPiece("");
                            this->board->clearIndicators();
                        } else {
                            this->board->setSelectedPiece(element.first);
                            this->gameState->kingCastleMove = 0ULL;
                            this->gameState->queenCastleMove = 0ULL;

                            auto selectedPiece = this->board->getSelectedPieceReference();
                            selectedPiece->rebuildValidMoves();
                            Bitboard validMoves = selectedPiece->getValidMoves() & ~this->gameState->calcOccupied();
                            this->checkCastle(selectedPiece);

                            Bitboard captureMoves = selectedPiece->getValidMoves() & this->gameState->calcBeatable(
                                    selectedPiece->getPieceColor()
                            );

                            this->checkPromotion(selectedPiece, captureMoves);

                            captureMoves = (captureMoves | this->gameState->getEnPassantMove()) &
                                           ~this->gameState->captureAndPromotionMove;

                            validMoves &= (~this->gameState->promotionMove);

                            this->board->drawValidMoves(
                                    validMoves,
                                    captureMoves,
                                    this->gameState->kingCastleMove,
                                    this->gameState->queenCastleMove,
                                    this->gameState->promotionMove,
                                    this->gameState->captureAndPromotionMove
                            );

                            this->gameState->setEnPassantMove(0LL);
                            this->gameState->kingCastleMove = 0ULL;
                            this->gameState->queenCastleMove = 0ULL;
                        }
                    }
                }
            }
        }
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
};