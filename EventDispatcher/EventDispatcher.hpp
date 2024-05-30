#pragma once

#include <unordered_map>
#include <queue>
#include <vector>
#include <SFML/Graphics.hpp>
#include <functional>
#include "EventListenerInterface.hpp"
#include "../Board/board.hpp"
#include "../GameState/GameState.hpp"

struct listenerStruct {
    EventListenerInterface* listenerInterface = nullptr;
    int priority = 0;

    listenerStruct() = default;

    listenerStruct(EventListenerInterface* listenerInterface, int priority)
            : listenerInterface(listenerInterface), priority(priority) {};

    bool operator>(const listenerStruct& other) const {
        return this->priority > other.priority;
    }

    bool operator<(const listenerStruct& other) const {
        return this->priority < other.priority;
    }
};

class EventDispatcher {
public:

    static void dispatch(const sf::Event& event) {
        try {
            auto rlisteners = EventDispatcher::listeners.at(event.type);
            while (!rlisteners.empty()) {
                auto listener = rlisteners.top();
                listener.listenerInterface->setContext(
                        EventDispatcher::board,
                        EventDispatcher::state,
                        EventDispatcher::window
                );

                listener.listenerInterface->onEvent(event);
                rlisteners.pop();
            }

        } catch (std::out_of_range& range) {
            return;
        }
    }

    static void registerListener(sf::Event::EventType eventType, EventListenerInterface* listener, int priority = 0) {
        EventDispatcher::listeners[eventType].emplace(listener, priority);
    }

    static void setContext(Board* gameBoard, GameState* gameState, sf::RenderWindow* window) {
        EventDispatcher::board = gameBoard;
        EventDispatcher::state = gameState;
        EventDispatcher::window = window;
    }

private:
    static std::unordered_map<
            sf::Event::EventType,
            std::priority_queue<listenerStruct, std::vector<listenerStruct>, std::greater<listenerStruct>>
    > listeners;
    static Board* board;
    static GameState* state;
    static sf::RenderWindow* window;
};

std::unordered_map<
        sf::Event::EventType,
        std::priority_queue<listenerStruct, std::vector<listenerStruct>, std::greater<listenerStruct>>
> EventDispatcher::listeners = {};
Board* EventDispatcher::board = nullptr;
GameState* EventDispatcher::state = nullptr;
sf::RenderWindow* EventDispatcher::window = nullptr;