#pragma once

#include <SFML/Graphics.hpp>
#include "../Board/board.hpp"
#include "../GameState/GameState.hpp"
#include "EventDispatcher.hpp"

class EventListenerInterface {
protected:
    virtual void onEvent(sf::Event event) = 0;

private:
    void setContext(Board* board, GameState* gameState, sf::RenderWindow* window) {
        this->board = board;
        this->gameState = gameState;
        this->window = window;
    }

protected:
    Board* board{};
    GameState* gameState{};
    sf::RenderWindow* window{};

    friend class EventDispatcher;
};