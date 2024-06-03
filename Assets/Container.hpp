#pragma once

#include "../Board/board.h"

class Board;

class Container {
public:
    static void buildContainer(
            Board* board,
            GameState* gameState,
            sf::RenderWindow* window
    ) {
        Container::gameState = gameState;
        Container::board = board;
        Container::window = window;
    }

    static void setGameSate(GameState* gameState) {
        Container::gameState = gameState;
    }

    static Board* getBoard() {
        return Container::board;
    }

    static GameState* getGameState() {
        return Container::gameState;
    }

    static sf::RenderWindow* getWindow() {
        return Container::window;
    }

private:
    static Board* board;
    static GameState* gameState;
    static sf::RenderWindow* window;
};

Board* Container::board = nullptr;
GameState* Container::gameState = nullptr;
sf::RenderWindow* Container::window = nullptr;