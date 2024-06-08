#pragma once

#include "../Board/board.cpp"

struct MoveInstruction {
public:
    int newX = 0;
    int newY = 0;
    int oldX = 0;
    int oldY = 0;

    Piece* selectedPiece;
    bool captured = false;
    bool copy = false;
    MoveOptions options;
    PieceColor pieceColor;
    PieceType selectedType;
    Board* board;
    GameState* gameState;

    MoveInstruction() = default;

    MoveInstruction(const MoveInstruction& other) {
        this->newX = other.newX;
        this->newY = other.newY;
        this->oldX = other.oldX;
        this->oldY = other.oldY;

        this->selectedPiece = other.selectedPiece;
        this->captured = other.captured;
        this->options = other.options;
        this->pieceColor = other.pieceColor;
        this->selectedType = other.selectedType;
        this->copy = true;
        this->gameState = new GameState(*other.gameState);
        this->board = new Board(*other.board, gameState);
    }

    ~MoveInstruction() {
        if (this->copy) {
            delete this->board;
            delete this->gameState;
        }
    }
};