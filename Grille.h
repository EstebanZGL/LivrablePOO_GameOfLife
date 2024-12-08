#ifndef GRILLE_H 
#define GRILLE_H 

#include <SFML/Graphics.hpp> 
#include <vector> 
#include <fstream>
#include "Cellule.h" 

class grille {
private:
    int ligne, colonne;
    float TailleCellule;
    std::vector<std::vector<Cellule>> cellules;

    int CompterVoisins(int x, int y) const;

public:
    float avoirTailleCellule() const;
    grille(int lig, int col, float taille);
    const std::vector<std::vector<Cellule>>& recupererCellule() const;
    void ouvrirFichier(const std::string& NomFichier);
    int avoirColonne() const;
    int avoirLigne() const;
    void defObstacle(int y, int x);
    void ModifierCellule(int sourisX, int sourisY);
    void GrilleMAJ();
    void ReinitialiserGrille();
    void DessinerGrille(sf::RenderWindow& fenetreSFML) const;
};

#endif // GRILLE_H
