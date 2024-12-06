#ifndef GRID_H 
#define GRID_H

#include <SFML/Graphics.hpp> 
#include <iostream>
#include <stdio.h>
#include <vector> 
#include <fstream>
#include "Cell.h"

class Grid {
private:
    int ligne, colonne; // Nombre de lignes et de colonnes.
    float cellSize; // Taille des cellules.
    std::vector<std::vector<Cell>> cells; // Matrice des cellules.
    bool torique; // Indique si la grille est torique.

    // Calcul des voisins vivants.
    int countNeighbors(int x, int y) const;

public:
    Grid(int lig, int col, float size, bool isTorique = true);

    float getCellSize() const; // Méthode pour obtenir la taille de la cellule.
    void setTorique(bool isTorique);
    bool isTorique() const;
    const std::vector<std::vector<Cell>>& getCells() const;

    void loadFromFile(const std::string& filename);
    int getcolonne() const;
    int getligne() const;

    void toggleObstacle(int y, int x);
    void toggleCell(int sourisX, int sourisY); // Change l'état d'une cellule en fonction des coordonnées de la souris.
    void updateGrid(); // Met à jour la grille.
    void clearGrid(); // Réinitialise la grille.
    void draw(sf::RenderWindow& window) const; // Dessine la grille dans une fenêtre SFML.
};

#endif
