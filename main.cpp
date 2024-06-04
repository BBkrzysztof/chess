#include <SFML/Graphics.hpp>
#include "Piece/Base/Piece.hpp"
#include "Board/board.cpp"
#include "EventDispatcher/EventDispatcher.hpp"
#include "EventDispatcher/Listeners/CloseWindowListener.hpp"
#include "EventDispatcher/Listeners/SelectPieceListener.hpp"
#include "EventDispatcher/Listeners/MovePieceListener.hpp"
#include "EventDispatcher/Listeners/CapturePieceListener.hpp"
#include "Moves/CheckEscape.hpp"

/**
 * @todo blokowanie innych ruchów wczasie szacha niż takie do ucieczki przed szachem
 * @todo check-mate stalemate
 * @return
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
        window.clear();


        sf::Event event;
        while (window.pollEvent(event)) {
            EventDispatcher::dispatch(event);
        }

        board->draw(window);

        if (gameState->getIsCheck() && gameState->checkEscapeMoves == FULL_BIT_BOARD) {
            CheckEscape::getCheckEscapeMoves(board, gameState);
        }

        board->isCheck();
        window.display();
    }

    return 0;
}
