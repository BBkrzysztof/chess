#pragma once

#include <unordered_map>
#include <vector>
#include <SFML/Graphics.hpp>
#include <functional>
#include "EventListenerInterface.hpp"
#include "../Board/board.hpp"
#include "../GameState/GameState.hpp"


class EventDispatcher {
public:

    static void dispatch(const sf::Event& event) {
        try {
            auto rlisteners = EventDispatcher::listeners.at(event.type);
            for (const auto& listener: rlisteners) {
                listener->setContext(
                        EventDispatcher::board,
                        EventDispatcher::state,
                        EventDispatcher::window
                );

                listener->onEvent(event);
            }

        } catch (std::out_of_range& range) {
            return;
        }
    }

    static void registerListener(sf::Event::EventType eventType, EventListenerInterface* listener) {
        EventDispatcher::listeners[eventType].push_back(listener);
    }

    static void setContext(Board* gameBoard, GameState* gameState, sf::RenderWindow* window) {
        EventDispatcher::board = gameBoard;
        EventDispatcher::state = gameState;
        EventDispatcher::window = window;
    }

private:
    static std::unordered_map<sf::Event::EventType, std::vector<EventListenerInterface*>> listeners;
    static Board* board;
    static GameState* state;
    static sf::RenderWindow* window;
};

std::unordered_map<sf::Event::EventType, std::vector<EventListenerInterface*>> EventDispatcher::listeners = {};
Board* EventDispatcher::board = nullptr;
GameState* EventDispatcher::state = nullptr;
sf::RenderWindow* EventDispatcher::window = nullptr;