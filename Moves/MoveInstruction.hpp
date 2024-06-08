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
    MoveOptions options;
    PieceColor pieceColor;
    PieceType selectedType;
    Board* board;
    GameState* gameState;

    MoveInstruction() = default;

    MoveInstruction(const MoveInstruction& other) {
        int newX = other.newX;
        int newY = other.newY;
        int oldX = other.oldX;
        int oldY = other.oldY;

        Piece* selectedPiece = other.selectedPiece;
        bool captured = other.captured;
        MoveOptions options = other.options;
        PieceColor pieceColor = other.pieceColor;
        PieceType selectedType = other.selectedType;
        Board* board = other.board;
        GameState* gameState = other.gameState;

    }

};