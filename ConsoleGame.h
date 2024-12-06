#ifndef CONSOLEGAME_HPP
#define CONSOLEGAME_HPP

#include <iostream>
#include <thread>
#include <chrono>
#include "Grid.h" // Inclusion de la classe Grid pour manipuler la grille.

class ConsoleGame {
private:
    Grid grid; // La grille de jeu.
    int delay; // Délai entre les itérations en millisecondes.
    bool torique; // Mode torique.

public:
    // Constructeur qui charge une grille à partir d'un fichier.
    ConsoleGame(const std::string& filename, int delayMs, bool isTorique)
        : grid(0, 0, 1.0f, isTorique), delay(delayMs), torique(isTorique) {
        grid.loadFromFile(filename);
    }

    // Constructeur pour un jeu avec des dimensions spécifiques.
    ConsoleGame(int ligne, int colonne, int delayMs, bool isTorique)
        : grid(ligne, colonne, 1.0f, isTorique), delay(delayMs), torique(isTorique) {}

    // Affiche la grille à la console.
    void printGrid() const {
        for (int x = 0; x < grid.getligne(); ++x) {
            for (int y = 0; y < grid.getcolonne(); ++y) {
                if (grid.getCells()[x][y].getAlive()) {
                    std::cout << "O"; // Cellule vivante.
                }
                else if (grid.getCells()[x][y].isObstacle()) {
                    std::cout << "#"; // Cellule obstacle.
                }
                else {
                    std::cout << "."; // Cellule morte.
                }
            }
            std::cout << std::endl; // Nouvelle ligne après chaque ligne de la grille.
        }
        std::cout << "====================================" << std::endl;
    }

    // Mise à jour de la grille.
    void updateGrid() {
        grid.updateGrid(); // Appel à la méthode de mise à jour de la grille de `Grid`.
    }

    // Lancement de la simulation en mode console.
    void start() {
        while (true) { // Boucle infinie, elle s'arrête lorsque l'utilisateur interrompt.
            printGrid(); // Affiche l'état actuel de la grille.
            updateGrid(); // Met à jour la grille selon les règles du Jeu de la Vie.
            std::this_thread::sleep_for(std::chrono::milliseconds(delay)); // Attente avant la prochaine itération.
        }
    }
};

#endif // CONSOLEGAME_HPP
