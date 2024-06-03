#pragma once

#include "../Board/board.cpp"

struct MoveInstruction {
public:
    int newX=0;
    int newY=0;
    int oldX=0;
    int oldY=0;

    Piece* selectedPiece;
    bool captured = false;
    MoveOptions options;
    PieceColor pieceColor;
    PieceType selectedType;
    Board* board;
    GameState* gameState;
};