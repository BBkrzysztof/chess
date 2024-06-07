#include <SFML/Graphics.hpp>
#include "Piece/Base/Piece.hpp"
#include "Board/board.cpp"
#include "EventDispatcher/EventDispatcher.hpp"
#include "EventDispatcher/Listeners/CloseWindowListener.hpp"
#include "EventDispatcher/Listeners/SelectPieceListener.hpp"
#include "EventDispatcher/Listeners/MovePieceListener.hpp"
#include "EventDispatcher/Listeners/CapturePieceListener.hpp"
#include "Engine/Engine.hpp"
/**
 * @todo better min-max
 */

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 800), "Chess Board");


    GameState* gameState = new GameState();
    Board* board = new Board(gameState);

    EventDispatcher::buildContext(board, gameState, &window);

    EventDispatcher::registerListener(
            sf::Event::Closed,
            new CloseWindowListener
    );

    EventDispatcher::registerListener(
            sf::Event::MouseButtonPressed,
            new SelectPieceListener,
            1000
    );

    EventDispatcher::registerListener(
            sf::Event::MouseButtonPressed,
            new MovePieceListener,
            100
    );

    EventDispatcher::registerListener(
            sf::Event::MouseButtonPressed,
            new CapturePieceListener,
            101
    );

    while (window.isOpen()) {

        sf::Event event;
        while (window.pollEvent(event)) {
            EventDispatcher::dispatch(event);
        }
        window.clear();
        board->draw(window);
        window.display();


        if (gameState->getIsCheckMate()) {
            std::cout << "Mat" << std::endl;
            return 0;
        }

        if (gameState->getTurn() == PieceColor::BLACK_PIECE) {
            { Engine::run(board, gameState, PieceColor::BLACK_PIECE, 3); }
        }
    }

    return 0;
}
