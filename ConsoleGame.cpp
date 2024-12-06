#include "ConsoleGame.h"

ConsoleGame::ConsoleGame(const std::string& filename, int delayMs, bool isTorique)
    : grid(0, 0, 1.0, isTorique), delay(delayMs), torique(isTorique) {
    grid.loadFromFile(filename);
}

ConsoleGame::ConsoleGame(int ligne, int colonne, int delayMs, bool isTorique)
    : grid(ligne, colonne, 1.0, isTorique), delay(delayMs), torique(isTorique) {}

void ConsoleGame::printGrid() const {
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
        std::cout << std::endl; // Nouvelle ligne apr�s chaque ligne de la grille.
    }
    std::cout << "====================================" << std::endl;
}

void ConsoleGame::updateGrid() {
    grid.updateGrid(); // Appel � la m�thode de mise � jour de la grille de `Grid`.
}

void ConsoleGame::start() {
    while (true) { // Boucle infinie, elle s'arr�te lorsque l'utilisateur interrompt.
        printGrid(); // Affiche l'�tat actuel de la grille.
        updateGrid(); // Met � jour la grille selon les r�gles du Jeu de la Vie.
        std::this_thread::sleep_for(std::chrono::milliseconds(delay)); // Attente avant la prochaine it�ration.
    }
}
