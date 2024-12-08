#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp> 
#include <iostream> 
#include <fstream> 
#include <vector> 
#include "grille.h" 

struct StructureData {
    std::vector<std::pair<int, int>> cellules;
};

class ModeGraphique {
private:
    grille grille;
    sf::RenderWindow fenetreSFML;
    sf::Font police;
    sf::Text TexteIteration;
    sf::Text touche;
    sf::Text touche2;
    sf::Text toucheT;

    bool running;
    bool editionGrille;
    int CompteurIteration;
    int delai;
    StructureData structureData;

public:
    ModeGraphique(int ligne, int colonne, float Taille, int delaiMs);
    ModeGraphique(const std::string& NomFichier, float Taille, int delaiMs);

    void CreerFenetre(const std::string& Contenu);
    void StructurePredefinies(const std::string& NomFichier);
    void GererTouches(sf::Event& evenement);
    void Demarrage();
};

#endif // GAME_HPP
