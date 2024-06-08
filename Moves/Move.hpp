#pragma once

#include "MoveInstruction.hpp"
//#include "../Engine/EvaluateBoard.hpp"

class Move {
public:
    static void move(const MoveInstruction* instruction) {
        auto selectedPiece = instruction->selectedPiece;
        auto gameState = instruction->gameState;
        auto board = instruction->board;
        auto captured = instruction->captured;

        int oldX = selectedPiece->getPositionX() / 100;
        int oldY = selectedPiece->getPositionY() / 100;

        int newX = instruction->newX;
        int newY = instruction->newY;

        Bitboard current = gameState->getBitBoard(
                selectedPiece->getPieceType(),
                selectedPiece->getPieceColor()
        );

        Bitboard newBitBoard = BitBoard::moveOnBitBoard(
                current,
                BitBoard::calcShift(oldX, oldY),
                BitBoard::calcShift(newX, newY)
        );

        selectedPiece->move(newX, newY);

        gameState->updateBitBoard(
                selectedPiece->getPieceType(),
                selectedPiece->getPieceColor(),
                newBitBoard
        );

        gameState->updateHistory(
                BitBoard::calcShift(oldX, oldY),
                BitBoard::calcShift(newX, newY),
                captured ? MoveOptions::Capture : MoveOptions::Move,
                selectedPiece
        );

        gameState->toggleTurn();
        board->setSelectedPiece("");
    }
};