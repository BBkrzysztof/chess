#pragma once

#include <unordered_map>
#include <queue>
#include <vector>
#include <SFML/Graphics.hpp>
#include <functional>
#include "EventListenerInterface.hpp"
#include "../Board/board.cpp"
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
        auto rlisteners = EventDispatcher::listeners[event.type];
        while (!rlisteners.empty()) {
            auto listener = rlisteners.top();
            listener.listenerInterface->setContext(
                    EventDispatcher::board,
                    EventDispatcher::gameState,
                    EventDispatcher::window
            );

            listener.listenerInterface->onEvent(event);
            rlisteners.pop();
        }
    }

    static void registerListener(sf::Event::EventType eventType, EventListenerInterface* listener, int priority = 0) {
        EventDispatcher::listeners[eventType].emplace(listener, priority);
    }

    static void buildContext(
            Board* board,
            GameState* gameState,
            sf::RenderWindow* window
    ) {
        EventDispatcher::gameState = gameState;
        EventDispatcher::board = board;
        EventDispatcher::window = window;
    }


private:
    static std::unordered_map<
            sf::Event::EventType,
            std::priority_queue<listenerStruct, std::vector<listenerStruct>>
    > listeners;

    static Board* board;
    static GameState* gameState;
    static sf::RenderWindow* window;
};

std::unordered_map<
        sf::Event::EventType,
        std::priority_queue<listenerStruct, std::vector<listenerStruct>>
> EventDispatcher::listeners = {};

Board* EventDispatcher::board = nullptr;
GameState* EventDispatcher::gameState = nullptr;
sf::RenderWindow* EventDispatcher::window = nullptr;