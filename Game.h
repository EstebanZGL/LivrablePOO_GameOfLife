#ifndef GAME_HPP
#define GAME_HPP

#include "Grid.h"
#include "Cell.h"

class Game {
private:
    Grid grid;
    sf::RenderWindow window;
    bool running;

public:
    Game(int rows, int cols, float cellSize)
        : grid(rows, cols, cellSize), window(sf::VideoMode(cols* cellSize, rows* cellSize), "Jeu de la Vie"), running(true) {
    }

    void start() {
        grid.initialize();

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
            }

            grid.updateGrid();

            window.clear();
            grid.draw(window);
            window.display();

            sf::sleep(sf::milliseconds(200));  // Pause pour ralentir la simulation
        }
    }
};

#endif
