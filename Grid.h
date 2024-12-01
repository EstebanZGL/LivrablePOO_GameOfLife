#ifndef GRID_HPP
#define GRID_HPP

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

    void toggleCell(int mouseX, int mouseY) {
        int x = mouseY / cellSize;
        int y = mouseX / cellSize;
        if (x >= 0 && x < rows && y >= 0 && y < cols) {
            bool currentState = cells[x][y].getAlive();
            cells[x][y].setAlive(!currentState);
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
        sf::RectangleShape cellShape(sf::Vector2f(cellSize, cellSize)); // Taille complète
        for (int x = 0; x < rows; ++x) {
            for (int y = 0; y < cols; ++y) {
                cellShape.setPosition(y * cellSize, x * cellSize);
                cellShape.setFillColor(cells[x][y].getAlive() ? sf::Color::Black : sf::Color::White);
                window.draw(cellShape);
            }
        }

        // Lignes pour la grille
        sf::RectangleShape line(sf::Vector2f(cols * cellSize, 1));
        line.setFillColor(sf::Color::Black);
        for (int i = 1; i < rows; ++i) {
            line.setPosition(0, i * cellSize);
            window.draw(line);
        }
        line.setSize(sf::Vector2f(1, rows * cellSize));
        for (int i = 1; i < cols; ++i) {
            line.setPosition(i * cellSize, 0);
            window.draw(line);
        }
    }
};

#endif
