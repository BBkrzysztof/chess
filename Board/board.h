#pragma once

#include <map>
#include <unordered_map>
#include <iostream>
#include <sstream>
#include <functional>
#include <SFML/Graphics.hpp>

#include "../GameState/GameState.hpp"

#include "../Piece/King.hpp"
#include "../Piece/Pawn.hpp"
#include "../Piece/Rook.hpp"
#include "../Piece/Bishop.hpp"
#include "../Piece/Knight.hpp"
#include "../Piece/Queen.hpp"
#include "../Piece/Base/Piece.hpp"
#include "../Piece/Factory/PieceFactory.hpp"
#include "../Assets/Helper.hpp"

typedef uint64_t Bitboard;

const sf::Color lightColor(240, 217, 181);
const sf::Color darkColor(181, 136, 99);
const int tileSize = 100;

const std::string WHITE_KING = "white_king";
const std::string BLACK_KING = "black_king";

class Board;

class Board {
public:
    Board(GameState* gameState);

    Board(const Board& board, GameState* gameStateCopy);

    void draw(sf::RenderTarget& target);

    std::map<std::string, Piece*> getPieces();

    void setSelectedPiece(const std::string& pieceHash);

    std::string getSelectedPiece();

    Piece* getSelectedPieceReference();

    void clearIndicators();

    std::vector<MoveIndicator*> getIndicators();

    void drawValidMoves(
            const Bitboard& validMoves,
            const Bitboard& captureMoves,
            const Bitboard& kingSideCastle,
            const Bitboard& queenSideCastle,
            const Bitboard& promotions,
            const Bitboard& captureAndPromotion
    );

    void isCheck();

    void setPieces(std::map<std::string, Piece*> pieces);

private:

    void registerPawns();

    void registerKnights();

    void registerBishops();

    void registerRooks();

    void registerQueens();

    void registerKings();

    static void registerPiecesGroup(const std::function<void(int, int)>& reg, const Bitboard& bitboard);

    void registerPiece(Piece* piece, std::string key = "");

public:
    std::map<std::string, Piece*> pieces;

    std::vector<MoveIndicator*> indicators;

    std::string selectedPiece;

    GameState* gameState;

    friend class GameState;
};