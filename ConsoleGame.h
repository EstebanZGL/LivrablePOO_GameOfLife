#ifndef CONSOLEGAME_HPP // Protection contre les inclusions multiples.
#define CONSOLEGAME_HPP // Définition de la macro CONSOLEGAME_HPP pour éviter les inclusions multiples.

#include <iostream> // Inclusion de la bibliothèque pour les entrées/sorties standard.
#include "Grid.h" // Inclusion de la classe Grid (grille).
#include <thread> // Inclusion pour gérer les threads.
#include <chrono> // Inclusion pour gérer les délais temporels.
#include <limits> // Inclusion pour définir les limites des types numériques.
#include <atomic> // Inclusion pour utiliser des variables atomiques.

class ConsoleGame {
private:
    Grid grid; // Instance de la grille pour le jeu.
    int iterationCount; // Compteur du nombre d'itérations.
    int delay; // Délai entre les mises à jour de la grille (en millisecondes).
    std::atomic<bool> running; // Variable atomique pour contrôler l'exécution.

public:
    ConsoleGame(int ligne, int colonne, int delayMs) // Constructeur initialisant une grille de taille donnée.
        : grid(ligne, colonne, 1.0), // Initialisation de la grille.
        iterationCount(0), delay(delayMs), running(true) {} // Initialisation des autres variables membres.

    ConsoleGame(const std::string& filename, int delayMs) // Constructeur initialisant une grille à partir d'un fichier.
        : grid(0, 0, 1.0), iterationCount(0), delay(delayMs), running(true) {
        grid.loadFromFile(filename); // Chargement de la grille depuis le fichier.
    }

    void displayGrid() const { // Méthode pour afficher la grille dans la console.
        printf("\033c"); // Efface la console.
        for (int x = 0; x < grid.getligne(); ++x) { // Parcours des lignes de la grille.
            for (int y = 0; y < grid.getcolonne(); ++y) { // Parcours des colonnes de la grille.
                if (grid.getCells()[x][y].getAlive()) {
                    std::cout << "\033[31m1\033[0m "; // Affiche un "1" en rouge si la cellule est vivante.
                }
                else {
                    std::cout << "0 "; // Affiche un "0" si la cellule est morte.
                }
            }
            std::cout << std::endl; // Nouvelle ligne après chaque rangée.
        }
        std::cout << "Iterations: " << iterationCount << std::endl; // Affiche le nombre d'itérations.
        std::cout << "Entrez sur 'q' pour quitter." << std::endl; // Message pour quitter le jeu.
    }

    void inputThread() { // Méthode pour gérer les entrées utilisateur dans un thread séparé.
        char input;
        while (running) { // Boucle continue tant que le jeu est en cours.
            std::cin >> input; // Lecture de l'entrée utilisateur.
            if (input == 'q') {
                running = false; // Arrête le jeu si 'q' est saisi.
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Ignore le reste de la ligne.
        }
    }

    void start() { // Méthode pour démarrer le jeu.
        std::thread inputThread(&ConsoleGame::inputThread, this); // Lancement du thread d'entrée utilisateur.
        while (running) { // Boucle principale du jeu tant que le jeu est en cours.
            displayGrid(); // Affiche la grille.
            grid.updateGrid(); // Met à jour l'état de la grille.
            iterationCount++; // Incrémente le compteur d'itérations.
            std::this_thread::sleep_for(std::chrono::milliseconds(delay)); // Pause entre les mises à jour.
        }
        inputThread.join(); // Attend la fin du thread d'entrée utilisateur.
    }
};

#endif // Fin de la protection contre les inclusions multiples.
