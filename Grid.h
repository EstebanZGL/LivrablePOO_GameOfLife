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

    Grid(int r, int c, float size) // Constructeur de la grille.
        : ligne(r), colonne(c), cellSize(size), cells(r, std::vector<Cell>(c)) {} // Initialise les dimensions et les cellules.

    const std::vector<std::vector<Cell>>& getCells() const { return cells; } // Retourne la grille des cellules.


    void loadFromFile(const std::string& filename) { // Charge une grille à partir d'un fichier.
        std::ifstream file(filename); 
        if (file.is_open()) { 
            file >> ligne >> colonne; 
            cells.resize(ligne, std::vector<Cell>(colonne)); // Redimensionne la grille.
            for (int x = 0; x < ligne; ++x) { 
                for (int y = 0; y < colonne; ++y) { 
                    int state;
                    file >> state; // Lit l'état de la cellule 
                    cells[x][y].setAlive(state == 1); // Définit l'état de la cellule.
                }
            }
            file.close(); 
        }
        else {
            throw std::runtime_error("Impossible d'ouvrir le fichier."); // Lève une exception en cas d'échec.
        }
    }

    int getcolonne() const { return colonne; }
    int getligne() const { return ligne; } 


    void toggleCell(int sourisX, int sourisY) { // Change l'état d'une cellule en fonction des coordonnées de la souris.
        int x = sourisY / cellSize;
        int y = sourisX / cellSize;
        if (x >= 0 && x < ligne && y >= 0 && y < colonne) { 
            bool currentState = cells[x][y].getAlive(); // Récupère l'état actuel de la cellule.
            cells[x][y].setAlive(!currentState); // Inverse l'état de la cellule.
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
                        next[x][y].setAlive(0);
                    }
                }
                cells = next; // Met � jour la grille avec le nouvel �tat.
        }
    

    void draw(sf::RenderWindow& window) const { // Dessine la grille dans une fen�tre SFML.
        sf::RectangleShape cellShape(sf::Vector2f(cellSize, cellSize)); // Cr�e une forme rectangulaire pour une cellule.
        for (int x = 0; x < ligne; ++x) { // Parcours des lignes.
            for (int y = 0; y < colonne; ++y) { // Parcours des colonnes.
                cellShape.setPosition(static_cast<float>(y) * cellSize, static_cast<float>(x) * cellSize); // Positionne la cellule.
                cellShape.setFillColor(cells[x][y].getAlive() ? sf::Color::Black : sf::Color::White); // Choisit la couleur en fonction de l'�tat.

                window.draw(cellShape); // Dessine la cellule.
            }
        }

        // Lignes pour la grille.
        sf::RectangleShape line(sf::Vector2f(ligne * cellSize, 1)); // Ligne.
        line.setFillColor(sf::Color::Black); // Couleur noire pour les lignes.
        for (int i = 1; i < ligne; ++i) { // Dessine les lignes .
            line.setPosition(0, static_cast<float>(i) * cellSize); // Positionne la ligne.
            window.draw(line); // Dessine la ligne.
        }
        line.setSize(sf::Vector2f(1, colonne * cellSize)); // colonne.
        for (int i = 1; i < colonne; ++i) { // Dessine les colonnes.
            line.setPosition(static_cast<float>(i) * cellSize, 0); // Positionne la colonne.
            window.draw(line); // Dessine la colonne.
        }
    }
};

#endif 
