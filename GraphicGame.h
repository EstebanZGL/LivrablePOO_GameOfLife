#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include "Grid.h"

class GraphicGame {
private:
    Grid grid;
    sf::RenderWindow window;
    sf::Font font;
    sf::Text iterationText;

    bool running;    // Indique si la simulation est en cours
    bool editing;    // Mode édition pour cliquer sur les cellules
    int iterationCount;
    int delay;

public:
    GraphicGame(int rows, int cols, float cellSize, int delayMs)
        : grid(rows, cols, cellSize),
        window(sf::VideoMode(cols* cellSize, rows* cellSize + 60), "Jeu de la Vie"),
        running(false), editing(true), iterationCount(0), delay(delayMs) {
        if (!font.loadFromFile("Roboto-Regular.ttf")) {
            throw std::runtime_error("Impossible de charger la police 'Roboto-Regular.ttf'.");
        }

        // Configuration du texte
        iterationText.setFont(font);
        iterationText.setCharacterSize(20);
        iterationText.setFillColor(sf::Color::Black);
        iterationText.setPosition(10, rows * cellSize + 10);
    }

    GraphicGame(const std::string& filename, float cellSize, int delayMS)
        : grid(0, 0, cellSize),
        window(sf::VideoMode(800, 600), "Jeu de la Vie"), // Taille par défaut
        running(false), editing(false), iterationCount(0), delay(delayMS) {
        grid.loadFromFile(filename);
        window.create(sf::VideoMode(grid.getCols() * cellSize, grid.getRows() * cellSize + 60), "Jeu de la Vie");

        if (!font.loadFromFile("Roboto-Regular.ttf")) {
            throw std::runtime_error("Impossible de charger la police 'Roboto-Regular.ttf'.");
        }

        // Configuration du texte
        iterationText.setFont(font);
        iterationText.setCharacterSize(20);
        iterationText.setFillColor(sf::Color::Black);
        iterationText.setPosition(10, grid.getRows() * cellSize + 10);
    }

    void handleInput(sf::Event& event) {
        if (event.type == sf::Event::Closed) {
            window.close();
        }
        else if (event.type == sf::Event::MouseButtonPressed && editing) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                grid.toggleCell(event.mouseButton.x, event.mouseButton.y);
            }
        }
        else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::P) {
                running = !running;
                editing = !running;
            }
        }
    }

    void start() {
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                handleInput(event);
            }

            if (running) {
                grid.updateGrid();
                iterationCount++;
                sf::sleep(sf::milliseconds(delay));
            }

            iterationText.setString("Iterations: " + std::to_string(iterationCount));

            window.clear(sf::Color::White);
            grid.draw(window);
            window.draw(iterationText);
            window.display();
        }
    }
};

#endif
