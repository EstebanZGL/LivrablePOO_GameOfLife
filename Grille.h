#ifndef GRID_HPP 
#define GRID_HPP 

#include <SFML/Graphics.hpp> 
#include <vector> 
#include <fstream>
#include "Cell.h" 

class grille {
private:
    int ligne, colonne; // Nombre de lignes et de colonnes de la grille.
    float TailleCellule; // Taille d'une cellule en pixels.
    std::vector<std::vector<Cell>> cellules; // Grille de cellules.

    int CompterVoisins(int x, int y) const { // compter les voisins vivants d'une cellule.
        int compte = 0; 
        for (int dx = -1; dx <= 1; ++dx) { 
            for (int dy = -1; dy <= 1; ++dy) { 
                if (dx == 0 && dy == 0) continue; 

                // Gestion des coordonnées en mode torique
                int nx = (x + dx + ligne) % ligne; 
                int ny = (y + dy + colonne) % colonne; 

                // Incrémente le compteur si le voisin est vivant.
                compte += cellules[nx][ny].estVivant();
            }
        }
        return compte; 
    }

public:

    float avoirTailleCellule() const { // Méthode pour obtenir la taille de la cellule
        return TailleCellule;
    }

    grille(int lig, int col, float taille) // Constructeur de la grille.
        : ligne(lig), colonne(col), TailleCellule(taille), cellules(lig, std::vector<Cell>(col)) {} // Initialise les dimensions et les cellules.

    const std::vector<std::vector<Cell>>& recupererCellule() const { return cellules; } // Retourne la grille des cellules.


    void ouvrirFichier(const std::string& NomFichier) {
        std::ifstream fichier(NomFichier);
        if (fichier.is_open()) {
            // Lire le nombre de lignes et de colonnes
            fichier >> ligne >> colonne;
            cellules.resize(ligne, std::vector<Cell>(colonne)); // Redimensionne la grille

            for (int x = 0; x < ligne; ++x) {
                for (int y = 0; y < colonne; ++y) {
                    int etat;
                    if (fichier >> etat) { // Tente de lire l'état de la cellule
                        if (etat == 1) {
                            cellules[x][y].defEtatCel(true); // Cellule vivante
                        }
                        else if (etat == 0) {
                            cellules[x][y].defEtatCel(false); // Cellule morte
                        }
                        else if (etat == 2 || etat == 3) {
                            // Traitement des obstacles : 
                            // Les cellules avec l'état '2' ou '3' seront des obstacles.
                            cellules[x][y].defObstacle(etat); // Déclenche la transformation en obstacle mort ou vivant
                        }
                        else {
                            cellules[x][y].defEtatCel(false); // Cellule morte
                        }
                    }
                }
                // Ignore les espaces restants dans la ligne
                fichier.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }

            fichier.close();
        }
        else {
            throw std::runtime_error("Impossible d'ouvrir le fichier."); // Lève une exception en cas d'échec
        }
    }


    int avoirColonne() const { return colonne; }
    int avoirLigne() const { return ligne; } 

    void defObstacle(int y, int x) {
        if (x >= 0 && x < ligne && y >= 0 && y < colonne) {
            cellules[x][y].defObstacle(); // Méthode à ajouter dans la classe Cell
        }
    }


    void ModifierCellule(int sourisX, int sourisY) { // Change l'état d'une cellule en fonction des coordonnées de la souris.
        int x = sourisY / TailleCellule;
        int y = sourisX / TailleCellule;
        if (x >= 0 && x < ligne && y >= 0 && y < colonne) {
            if (cellules[x][y].estModifiable()) { // Vérifie si la cellule peut être modifiée
                bool etatActuel = cellules[x][y].estVivant(); // Récupère l'état actuel de la cellule.
                cellules[x][y].defEtatCel(!etatActuel); // Inverse l'état de la cellule.
            }
        }
    }


    void GrilleMAJ() { // Met a jour la grille .
        std::vector<std::vector<Cell>> next = cellules; // Copie de l'état actuel des cellules.
        for (int x = 0; x < ligne; ++x) { 
            for (int y = 0; y < colonne; ++y) { 
                int voisins = CompterVoisins(x, y); // Compte les voisins vivants.
                if (cellules[x][y].estVivant()) { // Si la cellule est vivante.
                    next[x][y].defEtatCel(voisins == 2 || voisins == 3); // Survit si 2 ou 3 voisins, sinon meurt.
                }
                else { 
                    next[x][y].defEtatCel(voisins == 3); // Devient vivante si exactement 3 voisins.
                }

            }
       
        }
       cellules = next; // Met a jour la grille avec le nouvel etat.
    }
    
    void ReinitialiserGrille() {
        std::cout << "Reset de la grille" << std::endl;
        std::vector<std::vector<Cell>> next = cellules; // Copie de l'etat actuel des cellules.
                for (int x = 0; x < ligne; ++x) { // Parcours des lignes.
                    for (int y = 0; y < colonne; ++y) { // Parcours des colonnes.
                        next[x][y].ReinitialiserCellules();
                    }
                }
                cellules = next; // Met a jour la grille avec le nouvel etat.
        }
    

    void DessinerGrille(sf::RenderWindow& fenetreSFML) const { // Dessine la grille dans une fenêtre SFML.
        sf::RectangleShape FormeCellule(sf::Vector2f(TailleCellule, TailleCellule)); // Crée une forme rectangulaire pour une cellule.
        for (int x = 0; x < ligne; ++x) { // Parcours des lignes.
            for (int y = 0; y < colonne; ++y) { // Parcours des colonnes.
                FormeCellule.setPosition(static_cast<float>(y) * TailleCellule, static_cast<float>(x) * TailleCellule); // Positionne la cellule.

                // Choisit la couleur en fonction de l'état.
                if (cellules[x][y].estVivantCouleur()) {
                    FormeCellule.setFillColor(sf::Color::Black); // Couleur pour les cellules vivantes.
                }
                else if (cellules[x][y].estObstacleVivant()) {
                    FormeCellule.setFillColor(sf::Color::Green); // Couleur pour les obstacles vivants.
                }
                else if (cellules[x][y].estObstacle()) {
                    FormeCellule.setFillColor(sf::Color::Red); // Couleur pour les obstacles morts.
                }
                else {
                    FormeCellule.setFillColor(sf::Color::White); // Couleur pour les cellules mortes.
                }
                fenetreSFML.draw(FormeCellule); // Dessine la cellule.
            }
        }

        // Lignes pour la grille.
        sf::RectangleShape Posligne(sf::Vector2f(colonne * TailleCellule, 1)); // Ligne.
        Posligne.setFillColor(sf::Color::Black); // Couleur noire pour les lignes.

        for (int i = 1; i < ligne; ++i) { // Dessine les lignes.
            Posligne.setPosition(0, static_cast<float>(i) * TailleCellule); // Positionne la ligne.
            fenetreSFML.draw(Posligne); // Dessine la ligne.
        }

        Posligne.setSize(sf::Vector2f(1, ligne * TailleCellule)); // Colonne.

        for (int i = 1; i < colonne; ++i) { // Dessine les colonnes.
            Posligne.setPosition(static_cast<float>(i) * TailleCellule, 0); // Positionne la colonne.
            fenetreSFML.draw(Posligne); // Dessine la colonne.
        }
    }


};

#endif 

