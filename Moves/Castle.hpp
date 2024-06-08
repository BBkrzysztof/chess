#pragma once

#include "MoveInstruction.hpp"

class Castle {
public:
    static void castle(const MoveInstruction* instruction) {
        auto gameState = instruction->gameState;
        auto board = instruction->board;
        auto options = instruction->options;
        auto pieceColor = instruction->pieceColor;
        auto pieces = board->pieces;

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
                pieces.begin(),
                pieces.end(),
                [oldX, oldY](const std::pair<std::string, Piece*>& element) {
                    return element.second->getPositionX() == oldX * 100 && element.second->getPositionY() == oldY * 100;
                }
        );

        Piece* selectedPiece = result->second;
        Piece* king = pieces[pieceColor == PieceColor::WHITE_PIECE ? WHITE_KING : BLACK_KING];

        Bitboard rookBoard = gameState->getBitBoard(
                selectedPiece->getPieceType(),
                selectedPiece->getPieceColor()
        );

        Bitboard kingBoard = gameState->getBitBoard(
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

        gameState->updateBitBoard(
                selectedPiece->getPieceType(),
                selectedPiece->getPieceColor(),
                newRookBitBoard
        );

        gameState->updateBitBoard(
                king->getPieceType(),
                king->getPieceColor(),
                newKingBitBoard
        );

        if (pieceColor == PieceColor::WHITE_PIECE) {
            gameState->whiteCastleMoved = true;
        } else {
            gameState->blackCastleMoved = true;
        }

        gameState->updateHistory(
                0,
                0,
                options,
                king
        );

        gameState->toggleTurn();
        board->setSelectedPiece("");
    }
};