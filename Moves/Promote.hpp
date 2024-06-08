#pragma once

#include "MoveInstruction.hpp"
#include "Move.hpp"

class Promote {
public:
    static void promote(const MoveInstruction* instruction) {
        auto gameState = instruction->gameState;
        auto board = instruction->board;
        auto pieces = board->pieces;
        auto selectedPiece = instruction->selectedPiece;
        auto type = instruction->selectedType;

        int oldX = selectedPiece->getPositionX() / 100;
        int oldY = selectedPiece->getPositionY() / 100;

        Bitboard current = gameState->getBitBoard(
                selectedPiece->getPieceType(),
                selectedPiece->getPieceColor()
        );

        Bitboard newBitBoard = BitBoard::capture(current, BitBoard::calcShift(oldX, oldY));

        gameState->updateBitBoard(
                selectedPiece->getPieceType(),
                selectedPiece->getPieceColor(),
                newBitBoard
        );

        Piece* piece = PieceFactory::create(
                type,
                selectedPiece->getPieceColor(),
                selectedPiece->getPositionX() / 100,
                selectedPiece->getPositionY() / 100
        );

        piece->setHash(selectedPiece->getHash());

        Bitboard target = gameState->getBitBoard(
                type,
                selectedPiece->getPieceColor()
        );

        target = BitBoard::insert(
                target,
                BitBoard::calcShift(
                        selectedPiece->getPositionX() / 100,
                        selectedPiece->getPositionY() / 100
                )
        );

        gameState->updateBitBoard(type, selectedPiece->getPieceColor(), target);
        gameState->removeFromTeam(selectedPiece);
        gameState->addToTeam(piece);
        pieces[selectedPiece->getHash()] = piece;

        board->setPieces(pieces);

        MoveInstruction* newInstruction = new MoveInstruction(*instruction);
        newInstruction->selectedPiece = piece;

        Move::move(newInstruction);

        gameState->toggleTurn();
        board->setSelectedPiece("");
        delete selectedPiece;
    }
};