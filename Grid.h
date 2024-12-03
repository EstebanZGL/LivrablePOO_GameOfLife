#ifndef GRID_HPP
#define GRID_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <fstream>
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

    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (file.is_open()) {
            file >> rows >> cols; // Lire le nombre de lignes et colonnes
            cells.resize(rows, std::vector<Cell>(cols));
            for (int x = 0; x < rows; ++x) {
                for (int y = 0; y < cols; ++y) {
                    int state;
                    file >> state;
                    cells[x][y].setAlive(state == 1);
                }
            }
            file.close();
        }
        else {
            throw std::runtime_error("Impossible d'ouvrir le fichier.");
        }
    }

    int getCols() const { return cols; } // Ajout de getCols
    int getRows() const { return rows; } // Ajout de getRows

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
        sf::RectangleShape cellShape(sf::Vector2f(cellSize, cellSize));
        for (int x = 0; x < rows; ++x) {
            for (int y = 0; y < cols; ++y) {
                cellShape.setPosition(static_cast<float>(y) * cellSize, static_cast<float>(x) * cellSize);
                cellShape.setFillColor(cells[x][y].getAlive() ? sf::Color::Black : sf::Color::White);
                window.draw(cellShape);
            }
        }

        // Lignes pour la grille
        sf::RectangleShape line(sf::Vector2f(cols * cellSize, 1));
        line.setFillColor(sf::Color::Black);
        for (int i = 1; i < rows; ++i) {
            line.setPosition(0, static_cast<float>(i) * cellSize);
            window.draw(line);
        }
        line.setSize(sf::Vector2f(1, rows * cellSize));
        for (int i = 1; i < cols; ++i) {
            line.setPosition(static_cast<float>(i) * cellSize, 0);
            window.draw(line);
        }
    }
};

#endif
