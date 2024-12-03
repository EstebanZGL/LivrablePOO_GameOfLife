#ifndef CONSOLEGAME_HPP
#define CONSOLEGAME_HPP

#include <iostream>
#include "Grid.h"
#include <thread>
#include <chrono>
#include <limits>
#include <atomic>
#include <fstream> // Inclure pour les opérations de fichier

class ConsoleGame {
private:
    Grid grid;
    int iterationCount;
    int delay;
    std::atomic<bool> running; // Variable atomique pour contrôler l'exécution
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
        for (int x = 0; x < grid.getRows(); ++x) {
            for (int y = 0; y < grid.getCols(); ++y) {
                if (grid.getCells()[x][y].getAlive()) {
                    std::cout << "\033[31m1\033[0m "; // 1 en rouge
                }
                else {
                    std::cout << "0 "; // 0 en couleur par défaut
                }
            }
            std::cout << std::endl;
        }
        std::cout << "Iterations: " << iterationCount << std::endl;
        std::cout << "Entrez sur 'q' pour quitter." << std::endl;

        // Sauvegarder l'état actuel dans le fichier
        saveCurrentState();
    }

    void saveCurrentState() { // Ne pas marquer comme const
        if (outputFile.is_open()) {
            outputFile << "Itération: " << iterationCount << "\n";
            for (int x = 0; x < grid.getRows(); ++x) {
                for (int y = 0; y < grid.getCols(); ++y) {
                    outputFile << (grid.getCells()[x][y].getAlive() ? "1" : "0") << " ";
                }
                outputFile << "\n";
            }
            outputFile << "\n"; // Ligne vide entre les itérations
        }
    }

    void inputThread() {
        char input;
        while (running) {
            std::cin >> input;
            if (input == 'q') {
                running = false; // Met fin à l'exécution
            }
            // Ignore le reste de la ligne
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    void start() {
        std::thread inputThread(&ConsoleGame::inputThread, this); // Lancer le thread d'entrée
        while (running) {
            displayGrid();
            grid.updateGrid();
            iterationCount++;
            std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        }
        inputThread.join(); // Attendre que le thread d'entrée se termine
    }
};

#endif
