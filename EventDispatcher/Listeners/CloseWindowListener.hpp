#pragma once

#include "../EventListenerInterface.hpp"

class CloseWindowListener : public EventListenerInterface {
    void onEvent(sf::Event event) final {
        if (event.type == sf::Event::Closed)
            this->window->close();
    }
};