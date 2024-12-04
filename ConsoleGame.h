#ifndef CONSOLEGAME_HPP // Protection contre les inclusions multiples.
#define CONSOLEGAME_HPP // Définition de la macro CONSOLEGAME_HPP pour éviter les inclusions multiples.

#include <iostream>
#include "Grid.h"
#include <thread>
#include <chrono>
#include <limits>
#include <atomic>
#include <fstream> // Inclure pour les opÃ©rations de fichier

class ConsoleGame {
private:
    Grid grid;
    int iterationCount;
    int delay;
    std::atomic<bool> running; // Variable atomique pour contrÃ´ler l'exÃ©cution
    std::ofstream outputFile; // Fichier de sortie

public:
    ConsoleGame(int rows, int cols, int delayMs)
        : grid(rows, cols, 1.0f),
        iterationCount(0), delay(delayMs), running(true) {
        outputFile.open("sauvegarde.txt"); // Ouvrir le fichier de sortie
        if (!outputFile.is_open()) {
            throw std::runtime_error("Impossible d'ouvrir le fichier de sauvegarde.");
        }
    }

    ConsoleGame(const std::string& filename, int delayMs)
        : grid(0, 0, 1.0f), iterationCount(0), delay(delayMs), running(true) {
        grid.loadFromFile(filename);
        outputFile.open("sauvegarde.txt"); // Ouvrir le fichier de sortie
        if (!outputFile.is_open()) {
            throw std::runtime_error("Impossible d'ouvrir le fichier de sauvegarde.");
        }
    }

    ~ConsoleGame() {
        if (outputFile.is_open()) {
            outputFile.close(); // Fermer le fichier de sortie
        }
    }

    void displayGrid() {
        printf("\033c"); // Efface la console
        for (int x = 0; x < grid.getligne(); ++x) {
            for (int y = 0; y < grid.getcolonne(); ++y) {
                if (grid.getCells()[x][y].getAlive()) {
                    std::cout << "\033[31m1\033[0m "; // 1 en rouge
                }
                else {
                    std::cout << "0 "; // 0 en couleur par défaut

                }
            }
            std::cout << std::endl; // Nouvelle ligne après chaque rangée.
        }

        std::cout << "Iterations: " << iterationCount << std::endl;
        std::cout << "Entrez sur 'q' pour quitter." << std::endl;

        // Sauvegarder l'Ã©tat actuel dans le fichier
        saveCurrentState();
    }

    void saveCurrentState() { // Ne pas marquer comme const
        if (outputFile.is_open()) {
            outputFile << "ItÃ©ration: " << iterationCount << "\n";
            for (int x = 0; x < grid.getligne(); ++x) {
                for (int y = 0; y < grid.getcolonne(); ++y) {
                    outputFile << (grid.getCells()[x][y].getAlive() ? "1" : "0") << " ";
                }
                outputFile << "\n";
            }
            outputFile << "\n"; // Ligne vide entre les itÃ©rations
        }
    }

    void inputThread() {

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
