#ifndef CONSOLEGAME_HPP
#define CONSOLEGAME_HPP

#include <iostream>
#include "Grid.h"
#include <thread>
#include <chrono>
#include <limits>
#include <atomic>

class ConsoleGame {
private:
    Grid grid;
    int iterationCount;
    int delay;
    std::atomic<bool> running; // Variable atomique pour contrôler l'exécution

public:
    ConsoleGame(int rows, int cols, int delayMs)
        : grid(rows, cols, 1.0f),
        iterationCount(0), delay(delayMs), running(true) {
    }

    ConsoleGame(const std::string& filename, int delayMs)
        : grid(0, 0, 1.0f), iterationCount(0), delay(delayMs), running(true) {
        grid.loadFromFile(filename);
    }

    void displayGrid() const {
        printf("\033c"); // Efface la console
        for (int x = 0; x < grid.getRows(); ++x) {
            for (int y = 0; y < grid.getCols(); ++y) {
                if (grid.getCells()[x][y].getAlive()) {
                    // Afficher 1 en rouge
                    std::cout << "\033[31m1\033[0m "; // 31 est le code ANSI pour le rouge
                }
                else {
                    std::cout << "0 "; // Afficher 0 en couleur par défaut
                }
            }
            std::cout << std::endl;
        }
        std::cout << "Iterations: " << iterationCount << std::endl;
        std::cout << "Entrez sur 'q' pour quitter." << std::endl;
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
