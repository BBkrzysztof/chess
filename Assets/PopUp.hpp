#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

#include "PopUpInterface.hpp"

struct Panel {
    sf::Sprite sprite;
    sf::Texture texture;
    PieceColor pieceColor;
    PieceType pieceType;


    Panel(PieceType type, PieceColor color, int y) {
        this->pieceColor = color;
        this->pieceType = type;

        switch (type) {
            case PieceType::KNIGHT:
                if (!this->texture.loadFromFile(this->getUrl(PieceType::KNIGHT))) {
                    throw std::exception();
                }
                break;
            case PieceType::BISHOP:
                this->texture.loadFromFile(this->getUrl(PieceType::BISHOP));
                break;
            case PieceType::ROOK:
                this->texture.loadFromFile(this->getUrl(PieceType::ROOK));
                break;
            case PieceType::QUEEN:
                this->texture.loadFromFile(this->getUrl(PieceType::QUEEN));
                break;
        }
        this->sprite.setPosition(0.0f, (y * 100));
        this->sprite.setTexture(this->texture);
    }

    sf::Sprite getSprite() {
        return this->sprite;
    }

    PieceType getType() {
        return this->pieceType;
    }

private:
    std::string buildIconColor(const std::string& icon) {
        if (this->pieceColor == PieceColor::BLACK_PIECE) {
            return "_" + icon;
        }
        return icon;
    }

    std::string getUrl(PieceType type) {
        return Panel::PATH + this->buildIconColor(
                Panel::icons.at(type)
        );
    }

    const std::string PATH = "../Piece/icons/";

    const std::map<PieceType, std::string> icons = {
            {PieceType::KNIGHT, "N.png"},
            {PieceType::BISHOP, "B.png"},
            {PieceType::ROOK,   "R.png"},
            {PieceType::QUEEN,  "Q.png"},
    };
};


class PopUp : public PopUpInterface {
public:
    PopUp(const PieceColor& color) {
        this->pieceColor = color;

        this->buildPanel(PieceType::KNIGHT, 0);
        this->buildPanel(PieceType::BISHOP, 1);
        this->buildPanel(PieceType::ROOK, 2);
        this->buildPanel(PieceType::QUEEN, 3);
    }

    ~PopUp() {
        for (const auto& element: this->panels) {
            delete element;
        }

        this->panels.clear();
    }

    PieceType draw() {
        sf::RenderWindow newWindow(sf::VideoMode(100, 400), "promotion", sf::Style::None);

        while (newWindow.isOpen()) {

            newWindow.clear(sf::Color::White);
            for (auto& element: this->panels) {
                auto sprite = element->getSprite();

                newWindow.draw(sprite);
            }

            sf::Event event;
            while (newWindow.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    newWindow.close();
                }
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        sf::Vector2i mousePos = sf::Mouse::getPosition(newWindow);
                        for (auto& element: this->panels) {
                            auto sprite = element->getSprite();
                            if (sprite.getGlobalBounds().contains(static_cast<float>(mousePos.x),
                                                                  static_cast<float>(mousePos.y))) {
                                return element->getType();
                            }
                        }
                    }
                }
            }


            newWindow.display();
        }
    }

private:
    void buildPanel(PieceType type, int y) {
        Panel* panel = new Panel(type, this->pieceColor, y);
        this->panels.push_back(panel);
    }

private:

    std::vector<Panel*> panels;
    PieceColor pieceColor;
};