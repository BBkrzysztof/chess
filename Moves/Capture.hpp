#pragma once

#include "MoveInstruction.hpp"

class Capture {
public:
    static void capture(const MoveInstruction* instruction) {
        auto gameState = instruction->gameState;
        auto board = instruction->board;
        auto pieces = board->pieces;

        auto oldX = instruction->oldX;
        auto oldY = instruction->oldY;

        auto result = std::find_if(
                pieces.begin(),
                pieces.end(),
                [oldX, oldY](const std::pair<std::string, Piece*>& element) {
                    return element.second->getPositionX() == oldX * 100 && element.second->getPositionY() == oldY * 100;
                }
        );

        Piece* selectedPiece = nullptr;

        if (result != pieces.end()) {
            selectedPiece = result->second;
        } else {
            selectedPiece = gameState->peekHistory()->piece;
        }

        if (selectedPiece == nullptr) {
            throw std::exception();
        }

        Bitboard current = gameState->getBitBoard(
                selectedPiece->getPieceType(),
                selectedPiece->getPieceColor()
        );

        gameState->markFirstMove(selectedPiece);


        Bitboard newBitBoard = BitBoard::capture(current, BitBoard::calcShift(oldX, oldY));

        gameState->updateBitBoard(
                selectedPiece->getPieceType(),
                selectedPiece->getPieceColor(),
                newBitBoard
        );

        auto element = pieces.find(selectedPiece->getHash());
        pieces.erase(element);
        board->setPieces(pieces);
        gameState->removeFromTeam(selectedPiece);
        delete selectedPiece;
    }
};