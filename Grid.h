#ifndef GRID_HPP // Protection contre les inclusions multiples.
#define GRID_HPP // Définition de la macro GRID_HPP pour éviter les inclusions multiples.

#include <SFML/Graphics.hpp> // Inclusion de la bibliothèque SFML pour la gestion graphique.
#include <vector> // Inclusion de la bibliothèque pour les vecteurs dynamiques.
#include <fstream> // Inclusion pour les opérations sur les fichiers.
#include "Cell.h" // Inclusion de la classe Cell.

class Grid {
private:
    int ligne, colonne; // Nombre de lignes et de colonnes de la grille.
    float cellSize; // Taille d'une cellule en pixels.
    std::vector<std::vector<Cell>> cells; // Grille de cellules.

    int countNeighbors(int x, int y) const { // Méthode privée pour compter les voisins vivants d'une cellule.
        int count = 0; // Compteur de voisins vivants.
        for (int dx = -1; dx <= 1; ++dx) { // Parcours des 3 lignes autour de la cellule.
            for (int dy = -1; dy <= 1; ++dy) { // Parcours des 3 colonnes autour de la cellule.
                if (dx == 0 && dy == 0) continue; // Ignore la cellule elle-même.
                int nx = x + dx, ny = y + dy; // Coordonnées du voisin.
                if (nx >= 0 && nx < ligne && ny >= 0 && ny < colonne) { // Vérifie si le voisin est dans la grille.
                    count += cells[nx][ny].getAlive(); // Incrémente le compteur si le voisin est vivant.
                }
            }
        }
        return count; // Retourne le nombre de voisins vivants.
    }

public:
    Grid(int r, int c, float size) // Constructeur de la grille.
        : ligne(r), colonne(c), cellSize(size), cells(r, std::vector<Cell>(c)) {} // Initialise les dimensions et les cellules.

    const std::vector<std::vector<Cell>>& getCells() const { return cells; } // Retourne la grille des cellules.

    void loadFromFile(const std::string& filename) { // Charge une grille à partir d'un fichier.
        std::ifstream file(filename); // Ouvre le fichier en lecture.
        if (file.is_open()) { // Vérifie si le fichier est ouvert.
            file >> ligne >> colonne; // Lit le nombre de lignes et colonnes.
            cells.resize(ligne, std::vector<Cell>(colonne)); // Redimensionne la grille.
            for (int x = 0; x < ligne; ++x) { // Parcours des lignes.
                for (int y = 0; y < colonne; ++y) { // Parcours des colonnes.
                    int state;
                    file >> state; // Lit l'état de la cellule (0 ou 1).
                    cells[x][y].setAlive(state == 1); // Définit l'état de la cellule.
                }
            }
            file.close(); // Ferme le fichier.
        }
        else {
            throw std::runtime_error("Impossible d'ouvrir le fichier."); // Lève une exception en cas d'échec.
        }
    }

    int getcolonne() const { return colonne; } // Retourne le nombre de colonnes.
    int getligne() const { return ligne; } // Retourne le nombre de lignes.

    void toggleCell(int mouseX, int mouseY) { // Change l'état d'une cellule en fonction des coordonnées de la souris.
        int x = mouseY / cellSize; // Calcule l'index de la ligne.
        int y = mouseX / cellSize; // Calcule l'index de la colonne.
        if (x >= 0 && x < ligne && y >= 0 && y < colonne) { // Vérifie si les coordonnées sont valides.
            bool currentState = cells[x][y].getAlive(); // Récupère l'état actuel de la cellule.
            cells[x][y].setAlive(!currentState); // Inverse l'état de la cellule.
        }
    }

    void updateGrid() { // Met à jour la grille en appliquant les règles du jeu.
        std::vector<std::vector<Cell>> next = cells; // Copie de l'état actuel des cellules.
        for (int x = 0; x < ligne; ++x) { // Parcours des lignes.
            for (int y = 0; y < colonne; ++y) { // Parcours des colonnes.
                int neighbors = countNeighbors(x, y); // Compte les voisins vivants.
                if (cells[x][y].getAlive()) { // Si la cellule est vivante.
                    next[x][y].setAlive(neighbors == 2 || neighbors == 3); // Survit si 2 ou 3 voisins, sinon meurt.
                }
                else { // Si la cellule est morte.
                    next[x][y].setAlive(neighbors == 3); // Devient vivante si exactement 3 voisins.
                }
            }
        }
        cells = next; // Met à jour la grille avec le nouvel état.
    }

    void draw(sf::RenderWindow& window) const { // Dessine la grille dans une fenêtre SFML.
        sf::RectangleShape cellShape(sf::Vector2f(cellSize, cellSize)); // Crée une forme rectangulaire pour une cellule.
        for (int x = 0; x < ligne; ++x) { // Parcours des lignes.
            for (int y = 0; y < colonne; ++y) { // Parcours des colonnes.
                cellShape.setPosition(static_cast<float>(y) * cellSize, static_cast<float>(x) * cellSize); // Positionne la cellule.
                cellShape.setFillColor(cells[x][y].getAlive() ? sf::Color::Black : sf::Color::White); // Choisit la couleur en fonction de l'état.
                window.draw(cellShape); // Dessine la cellule.
            }
        }

        // Lignes pour la grille.
        sf::RectangleShape line(sf::Vector2f(colonne * cellSize, 1)); // Ligne horizontale.
        line.setFillColor(sf::Color::Black); // Couleur noire pour les lignes.
        for (int i = 1; i < ligne; ++i) { // Dessine les lignes horizontales.
            line.setPosition(0, static_cast<float>(i) * cellSize); // Positionne la ligne.
            window.draw(line); // Dessine la ligne.
        }
        line.setSize(sf::Vector2f(1, ligne * cellSize)); // Ligne verticale.
        for (int i = 1; i < colonne; ++i) { // Dessine les lignes verticales.
            line.setPosition(static_cast<float>(i) * cellSize, 0); // Positionne la ligne.
            window.draw(line); // Dessine la ligne.
        }
    }
};

#endif // Fin de la protection contre les inclusions multiples.
