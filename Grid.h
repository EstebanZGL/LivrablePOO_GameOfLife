#ifndef GRID_HPP 
#define GRID_HPP 

#include <SFML/Graphics.hpp> 
#include <vector> 
#include <fstream>
#include "Cell.h" 

class Grid {
private:
    int ligne, colonne; // Nombre de lignes et de colonnes de la grille.
    float cellSize; // Taille d'une cellule en pixels.
    std::vector<std::vector<Cell>> cells; // Grille de cellules.
    bool torique; // Indique si la grille est torique.

    int countNeighbors(int x, int y) const { // compter les voisins vivants d'une cellule.
        int count = 0; 
        for (int dx = -1; dx <= 1; ++dx) { 
            for (int dy = -1; dy <= 1; ++dy) { 
                if (dx == 0 && dy == 0) continue; 

                // Gestion des coordonnées en mode torique
                int nx = (x + dx + ligne) % ligne; 
                int ny = (y + dy + colonne) % colonne; 

                // Incrémente le compteur si le voisin est vivant.
                count += cells[nx][ny].getAlive();
            }
        }
        return count; 
    }


public:

    float getCellSize() const { // M�thode pour obtenir la taille de la cellule
        return cellSize;
    }

    Grid(int lig, int col, float size, bool isTorique)
        : ligne(lig), colonne(col), cellSize(size), cells(lig, std::vector<Cell>(col)), torique(isTorique) {
    }

    const std::vector<std::vector<Cell>>& getCells() const { return cells; } // Retourne la grille des cellules.

    bool getTorique() const{
        return torique;
    }

    void setTorique(bool isTorique) {
        torique = isTorique;
    }

    


    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (file.is_open()) {
            // Lire le nombre de lignes et de colonnes
            file >> ligne >> colonne;
            cells.resize(ligne, std::vector<Cell>(colonne)); // Redimensionne la grille

            for (int x = 0; x < ligne; ++x) {
                for (int y = 0; y < colonne; ++y) {
                    int state;
                    if (file >> state) { // Tente de lire l'état de la cellule
                        if (state == 1) {
                            cells[x][y].setAlive(true); // Cellule vivante
                        }
                        else if (state == 0) {
                            cells[x][y].setAlive(false); // Cellule morte
                        }
                        else if (state == 2 || state == 3) {
                            // Traitement des obstacles : 
                            // Les cellules avec l'état '2' ou '3' seront des obstacles.
                            cells[x][y].toggleObstacle(state); // Déclenche la transformation en obstacle mort ou vivant
                        }
                        else {
                            cells[x][y].setAlive(false); // Cellule morte
                        }
                    }
                }
                // Ignore les espaces restants dans la ligne
                file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }

            file.close();
        }
        else {
            throw std::runtime_error("Impossible d'ouvrir le fichier."); // Lève une exception en cas d'échec
        }
    }


    int getcolonne() const { return colonne; }
    int getligne() const { return ligne; } 

    void toggleObstacle(int y, int x) {
        if (x >= 0 && x < ligne && y >= 0 && y < colonne) {
            cells[x][y].toggleObstacle(); // Méthode à ajouter dans la classe Cell
        }
    }

       void toggleCell(int sourisX, int sourisY) { // Change l'état d'une cellule en fonction des coordonnées de la souris.
        int x = sourisY / cellSize;
        int y = sourisX / cellSize;
        if (x >= 0 && x < ligne && y >= 0 && y < colonne) {
            if (cells[x][y].canBeModified()) { // Vérifie si la cellule peut être modifiée
                bool currentState = cells[x][y].getAlive(); // Récupère l'état actuel de la cellule.
                cells[x][y].setAlive(!currentState); // Inverse l'état de la cellule.
            }
        }
    }


    void updateGrid() { // Met à jour la grille .
        std::vector<std::vector<Cell>> next = cells; // Copie de l'état actuel des cellules.
        for (int x = 0; x < ligne; ++x) { 
            for (int y = 0; y < colonne; ++y) { 
                int neighbors = countNeighbors(x, y); // Compte les voisins vivants.
                if (cells[x][y].getAlive()) { // Si la cellule est vivante.
                    next[x][y].setAlive(neighbors == 2 || neighbors == 3); // Survit si 2 ou 3 voisins, sinon meurt.
                }
                else { 
                    next[x][y].setAlive(neighbors == 3); // Devient vivante si exactement 3 voisins.
                }

            }
       
        }
       cells = next; // Met � jour la grille avec le nouvel �tat.
    }
    
    void clearGrid() {
        std::cout << "Reset de la grille" << std::endl;
        std::vector<std::vector<Cell>> next = cells; // Copie de l'�tat actuel des cellules.
                for (int x = 0; x < ligne; ++x) { // Parcours des lignes.
                    for (int y = 0; y < colonne; ++y) { // Parcours des colonnes.
                        next[x][y].clearCell();
                    }
                }
                cells = next; // Met � jour la grille avec le nouvel �tat.
        }
    

    void draw(sf::RenderWindow& window) const { // Dessine la grille dans une fenêtre SFML.
        sf::RectangleShape cellShape(sf::Vector2f(cellSize, cellSize)); // Crée une forme rectangulaire pour une cellule.
        for (int x = 0; x < ligne; ++x) { // Parcours des lignes.
            for (int y = 0; y < colonne; ++y) { // Parcours des colonnes.
                cellShape.setPosition(static_cast<float>(y) * cellSize, static_cast<float>(x) * cellSize); // Positionne la cellule.

                // Choisit la couleur en fonction de l'état.
                if (cells[x][y].getAliveColor()) {
                    cellShape.setFillColor(sf::Color::Black); // Couleur pour les cellules vivantes.
                }
                else if (cells[x][y].isObstacleAlive()) {
                    cellShape.setFillColor(sf::Color::Green); // Couleur pour les obstacles vivants.
                }
                else if (cells[x][y].isObstacle()) {
                    cellShape.setFillColor(sf::Color::Red); // Couleur pour les obstacles morts.
                }
                else {
                    cellShape.setFillColor(sf::Color::White); // Couleur pour les cellules mortes.
                }

                window.draw(cellShape); // Dessine la cellule.
            }
        }

        // Lignes pour la grille.
        sf::RectangleShape line(sf::Vector2f(colonne * cellSize, 1)); // Ligne.
        line.setFillColor(sf::Color::Black); // Couleur noire pour les lignes.

        for (int i = 1; i < ligne; ++i) { // Dessine les lignes.
            line.setPosition(0, static_cast<float>(i) * cellSize); // Positionne la ligne.
            window.draw(line); // Dessine la ligne.
        }

        line.setSize(sf::Vector2f(1, ligne * cellSize)); // Colonne.

        for (int i = 1; i < colonne; ++i) { // Dessine les colonnes.
            line.setPosition(static_cast<float>(i) * cellSize, 0); // Positionne la colonne.
            window.draw(line); // Dessine la colonne.
        }
    }


};

#endif 

