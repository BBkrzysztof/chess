#pragma once

#include "../EventListenerInterface.hpp"

class CloseWindowListener : public EventListenerInterface {
    void onEvent(sf::Event event) {
        if (event.type == sf::Event::Closed)
            this->window->close();
    }
};