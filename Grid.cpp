#include "Grid.h"

Grid::Grid(int lig, int col, float size, bool isTorique)
    : ligne(lig), colonne(col), cellSize(size), cells(lig, std::vector<Cell>(col)), torique(isTorique) {}

float Grid::getCellSize() const {
    return cellSize;
}

void Grid::setTorique(bool isTorique) {
    torique = isTorique;
}

bool Grid::isTorique() const {
    return torique;
}

const std::vector<std::vector<Cell>>& Grid::getCells() const {
    return cells;
}

void Grid::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        // Lire le nombre de lignes et de colonnes
        file >> ligne >> colonne;
        cells.resize(ligne, std::vector<Cell>(colonne)); // Redimensionne la grille

        for (int x = 0; x < ligne; ++x) {
            for (int y = 0; y < colonne; ++y) {
                int state;
                if (file >> state) {
                    if (state == 1) {
                        cells[x][y].setAlive(true); // Cellule vivante
                    }
                    else if (state == 0) {
                        cells[x][y].setAlive(false); // Cellule morte
                    }
                    else if (state == 2 || state == 3) {
                        cells[x][y].toggleObstacle(state); // Les cellules avec état '2' ou '3' deviennent des obstacles.
                    }
                    else {
                        cells[x][y].setAlive(false); // Cellule morte
                    }
                }
            }
            file.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore les espaces restants dans la ligne
        }

        file.close();
    }
    else {
        throw std::runtime_error("Impossible d'ouvrir le fichier."); // Lève une exception si le fichier ne s'ouvre pas.
    }
}

int Grid::getcolonne() const {
    return colonne;
}

int Grid::getligne() const {
    return ligne;
}

void Grid::toggleObstacle(int y, int x) {
    if (x >= 0 && x < ligne && y >= 0 && y < colonne) {
        cells[x][y].toggleObstacle(); // Méthode à ajouter dans la classe Cell
    }
}

void Grid::toggleCell(int sourisX, int sourisY) {
    int x = sourisY / cellSize;
    int y = sourisX / cellSize;
    if (x >= 0 && x < ligne && y >= 0 && y < colonne) {
        if (cells[x][y].canBeModified()) {
            bool currentState = cells[x][y].getAlive();
            cells[x][y].setAlive(!currentState); // Inverse l'état de la cellule.
        }
    }
}

void Grid::updateGrid() {
    std::vector<std::vector<Cell>> next = cells; // Copie de l'état actuel des cellules.
    for (int x = 0; x < ligne; ++x) {
        for (int y = 0; y < colonne; ++y) {
            int neighbors = countNeighbors(x, y);
            if (cells[x][y].getAlive()) {
                next[x][y].setAlive(neighbors == 2 || neighbors == 3); // Survit si 2 ou 3 voisins vivants.
            }
            else {
                next[x][y].setAlive(neighbors == 3); // Devient vivante si exactement 3 voisins.
            }
        }
    }
    cells = next; // Mise à jour de la grille.
}

void Grid::clearGrid() {
    std::cout << "Reset de la grille" << std::endl;
    std::vector<std::vector<Cell>> next = cells;
    for (int x = 0; x < ligne; ++x) {
        for (int y = 0; y < colonne; ++y) {
            next[x][y].clearCell(); // Réinitialise chaque cellule.
        }
    }
    cells = next; // Mise à jour de la grille.
}

void Grid::draw(sf::RenderWindow& window) const {
    sf::RectangleShape cellShape(sf::Vector2f(cellSize, cellSize)); // Création d'une cellule sous forme de rectangle.
    for (int x = 0; x < ligne; ++x) {
        for (int y = 0; y < colonne; ++y) {
            cellShape.setPosition(static_cast<float>(y) * cellSize, static_cast<float>(x) * cellSize);

            if (cells[x][y].getAliveColor()) {
                cellShape.setFillColor(sf::Color::Black); // Cellule vivante en noir.
            }
            else if (cells[x][y].isObstacleAlive()) {
                cellShape.setFillColor(sf::Color::Green); // Obstacle vivant en vert.
            }
            else if (cells[x][y].isObstacle()) {
                cellShape.setFillColor(sf::Color::Red); // Obstacle mort en rouge.
            }
            else {
                cellShape.setFillColor(sf::Color::White); // Cellule morte en blanc.
            }

            window.draw(cellShape); // Dessine la cellule sur la fenêtre SFML.
        }
    }

    // Dessin des lignes de la grille.
    sf::RectangleShape line(sf::Vector2f(colonne * cellSize, 1));
    line.setFillColor(sf::Color::Black);
    for (int i = 1; i < ligne; ++i) {
        line.setPosition(0, static_cast<float>(i) * cellSize);
        window.draw(line);
    }

    line.setSize(sf::Vector2f(1, ligne * cellSize));
    for (int i = 1; i < colonne; ++i) {
        line.setPosition(static_cast<float>(i) * cellSize, 0);
        window.draw(line);
    }
}

int Grid::countNeighbors(int x, int y) const {
    int count = 0;
    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue;

            int nx = x + dx;
            int ny = y + dy;

            if (torique) {
                nx = (nx + ligne) % ligne;
                ny = (ny + colonne) % colonne;
            }
            else if (nx < 0 || nx >= ligne || ny < 0 || ny >= colonne) {
                continue;
            }

            count += cells[nx][ny].getAlive();
        }
    }
    return count;
}
