#ifndef GRAPHIC_GAME_H
#define GRAPHIC_GAME_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include "Grid.h"

struct StructureData {
    std::vector<std::pair<int, int>> cells; // Liste de cellules à activer.
};

class GraphicGame {
private:
    Grid grid;                        // Instance de la grille.
    sf::RenderWindow window;          // Fenêtre SFML.
    sf::Font font;                    // Police pour le texte.
    sf::Text iterationText;           // Texte pour afficher le nombre d'itérations.
    sf::Text toriqueText;             // Texte pour afficher le mode torique.
    sf::Text fonctionText;
    StructureData structureData;      // Données pour les structures chargées depuis un fichier.
    bool running;                     // Simulation en cours ou non.
    bool editing;                     // Mode édition activé ou non.
    int iterationCount;               // Nombre d'itérations effectuées.
    int delay;                        // Temps en millisecondes entre les mises à jour.

    void updateToriqueText();

public:
    GraphicGame(int ligne, int colonne, float cellSize, int delayMs, bool torique);
    GraphicGame(const std::string& filename, float cellSize, int delayMs, bool torique);

    void handleInput(sf::Event& event);
    void start();
};

#endif
