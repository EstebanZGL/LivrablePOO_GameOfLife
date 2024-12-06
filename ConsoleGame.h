#ifndef CONSOLEGAME_H
#define CONSOLEGAME_H

#include <iostream>
#include <thread>
#include <chrono>
#include "Grid.h"

class ConsoleGame {
private:
    Grid grid; // La grille de jeu.
    int delay; // Délai entre les itérations en millisecondes.
    bool torique; // Mode torique.

public:
    // Constructeur qui charge une grille à partir d'un fichier.
    ConsoleGame(const std::string& filename, int delayMs, bool isTorique);

    // Constructeur pour un jeu avec des dimensions spécifiques.
    ConsoleGame(int ligne, int colonne, int delayMs, bool isTorique);

    // Affiche la grille à la console.
    void printGrid() const;

    // Mise à jour de la grille.
    void updateGrid();

    // Lancement de la simulation en mode console.
    void start();
};

#endif
