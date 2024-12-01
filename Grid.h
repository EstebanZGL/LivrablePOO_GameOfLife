#ifndef GRID_HPP
#define GRID_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include "Cell.h"

class Grid {
private:
    int rows, cols;
    float cellSize;
    std::vector<std::vector<Cell>> cells;

    int countNeighbors(int x, int y) const {
        int count = 0;
        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                if (dx == 0 && dy == 0) continue;
                int nx = x + dx, ny = y + dy;
                if (nx >= 0 && nx < rows && ny >= 0 && ny < cols) {
                    count += cells[nx][ny].getAlive();
                }
            }
        }
        return count;
    }

public:
    Grid(int r, int c, float size) : rows(r), cols(c), cellSize(size), cells(r, std::vector<Cell>(c)) {}

    void initialize() {
        int x, y;
        std::cout << "Entrez les positions des cellules vivantes (x y), -1 -1 pour finir:\n";
        while (true) {
            std::cin >> x >> y;
            if (x == -1 && y == -1) break;
            if (x >= 0 && x < rows && y >= 0 && y < cols) {
                cells[x][y].setAlive(true);
            }
        }
    }

    void updateGrid() {
        std::vector<std::vector<Cell>> next = cells;
        for (int x = 0; x < rows; ++x) {
            for (int y = 0; y < cols; ++y) {
                int neighbors = countNeighbors(x, y);
                if (cells[x][y].getAlive()) {
                    next[x][y].setAlive(neighbors == 2 || neighbors == 3);
                }
                else {
                    next[x][y].setAlive(neighbors == 3);
                }
            }
        }
        cells = next;
    }

    void draw(sf::RenderWindow& window) const {
        sf::RectangleShape cellShape(sf::Vector2f(cellSize - 1, cellSize - 1));
        for (int x = 0; x < rows; ++x) {
            for (int y = 0; y < cols; ++y) {
                cellShape.setPosition(y * cellSize, x * cellSize);
                cellShape.setFillColor(cells[x][y].getAlive() ? sf::Color::Green : sf::Color::Black);
                window.draw(cellShape);
            }
        }
    }
};

#endif
