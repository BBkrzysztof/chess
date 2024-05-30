#include <SFML/Graphics.hpp>
#include <limits>
#include "Piece/Base/Piece.hpp"
#include "Piece/King.hpp"
#include "Board/board.hpp"
#include "EventDispatcher/EventDispatcher.hpp"
#include "EventDispatcher/Listeners/CloseWindowListener.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 800), "Chess Board");

    auto* k1 = new King(PieceColor::BLACK_PIECE, 0, 0);
    auto* k2 = new King(PieceColor::WHITE_PIECE, 700, 700);

    Board* board = new Board();
    GameState* gameState = new GameState();

    k2->toggleSelect();

    EventDispatcher::setContext(board, gameState, &window);

    EventDispatcher::registerListener(
            sf::Event::Closed,
            new CloseWindowListener
    );

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            EventDispatcher::dispatch(event);
        }

//        window.clear();
//
//        Board::draw(window);
//
//        k1->draw(window);
//        k2->draw(window);
//
//        window.display();
    }

    return 0;
}
