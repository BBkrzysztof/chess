#include <SFML/Graphics.hpp>
#include "Piece/Base/Piece.hpp"
#include "Board/board.hpp"
#include "EventDispatcher/EventDispatcher.hpp"
#include "EventDispatcher/Listeners/CloseWindowListener.hpp"
#include "EventDispatcher/Listeners/SelectPieceListener.hpp"
#include "EventDispatcher/Listeners/HoverPieceListener.hpp"
#include "EventDispatcher/Listeners/MovePieceListener.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 800), "Chess Board");


    auto* gameState = new GameState();
    auto* board = new Board(gameState);

    EventDispatcher::setContext(board, gameState, &window);

    EventDispatcher::registerListener(
            sf::Event::Closed,
            new CloseWindowListener
    );

    EventDispatcher::registerListener(
            sf::Event::MouseButtonPressed,
            new SelectPieceListener
    );

    EventDispatcher::registerListener(
            sf::Event::MouseButtonPressed,
            new MovePieceListener,
            100
    );


    while (window.isOpen()) {
        window.clear();

        sf::Event event;
        while (window.pollEvent(event)) {
            EventDispatcher::dispatch(event);
        }

        board->draw(window);

        window.display();
    }

    return 0;
}
