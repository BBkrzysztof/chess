#pragma once

#include <unordered_map>
#include <queue>
#include <vector>
#include <SFML/Graphics.hpp>
#include <functional>
#include "EventListenerInterface.hpp"
#include "../Board/board.hpp"
#include "../GameState/GameState.hpp"
#include "../Assets/Container.hpp"

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
                    Container::getBoard(),
                    Container::getGameState(),
                    Container::getWindow()
            );

            listener.listenerInterface->onEvent(event);
            rlisteners.pop();
        }
    }

    static void registerListener(sf::Event::EventType eventType, EventListenerInterface* listener, int priority = 0) {
        EventDispatcher::listeners[eventType].emplace(listener, priority);
    }

private:
    static std::unordered_map<
            sf::Event::EventType,
            std::priority_queue<listenerStruct, std::vector<listenerStruct>>
    > listeners;

};

std::unordered_map<
        sf::Event::EventType,
        std::priority_queue<listenerStruct, std::vector<listenerStruct>>
> EventDispatcher::listeners = {};
