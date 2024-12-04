

#define CONSOLEGAME_HPP // Définition de la macro CONSOLEGAME_HPP pour éviter de multiples inclusions.

#include <iostream> // Inclusion pour les entrées/sorties standard.
#include "Grid.h" // Inclusion de la classe Grid.
#include <thread> // Inclusion pour la gestion des threads.
#include <chrono> // Inclusion pour la gestion du temps et des délais.
#include <limits> // Inclusion pour gérer les limites des types numériques.
#include <atomic> // Inclusion pour les variables atomiques (thread-safe).
#include <fstream> // Inclusion pour les opérations sur les fichiers.


class ConsoleGame { // Classe représentant le jeu en mode console.
private:

    Grid grid; // Instance de la grille représentant l'état du jeu.
    int iterationCount; // Compteur d'itérations.
    int delay; // Délai entre chaque mise à jour de la grille (en millisecondes).
    std::atomic<bool> running; // Variable atomique pour contrôler l'exécution du jeu (thread-safe).
    std::ofstream outputFile; // Flux de fichier pour sauvegarder les états du jeu.

public:
    // Constructeur pour initialiser le jeu avec une grille vide.
    ConsoleGame(int ligne, int colonne, int delayms)
        : grid(ligne, colonne, 1.0), 
        iterationCount(0), delay(delayms), running(true) { 
        outputFile.open("sauvegarde.txt"); // Ouvre un fichier pour sauvegarder les états.
        if (!outputFile.is_open()) { // Vérifie si le fichier s'est bien ouvert.
            throw std::runtime_error("Impossible d'ouvrir le fichier de sauvegarde."); 
        }
    }

    // Constructeur pour initialiser le jeu à partir d'un fichier existant.
    ConsoleGame(const std::string& filename, int delayms)
        : grid(0, 0, 1.0f), iterationCount(0), delay(delayms), running(true) { 
        grid.loadFromFile(filename); 
        outputFile.open("sauvegarde.txt"); // Ouvre un fichier pour sauvegarder les états.
        if (!outputFile.is_open()) { // Vérifie si le fichier s'est bien ouvert.
            throw std::runtime_error("Impossible d'ouvrir le fichier de sauvegarde.");
        }
    }

    // Destructeur pour fermer le fichier de sortie si nécessaire.
    ~ConsoleGame() {
        if (outputFile.is_open()) { 
            outputFile.close(); 
        }
    }

    // Méthode pour afficher la grille dans la console.
    void displayGrid() {
        printf("\033c"); // Efface la console.
        for (int x = 0; x < grid.getligne(); ++x) { 
            for (int y = 0; y < grid.getcolonne(); ++y) { 
                if (grid.getCells()[x][y].getAlive()) { // Si la cellule est vivante.
                    std::cout << "\033[31m1\033[0m "; // Affiche "1" en rouge 033=1 en ESC.
                }
                else {
                    std::cout << "0 "; 
                }
            }
            std::cout << std::endl; 
        }

        
        std::cout << "Iterations: " << iterationCount << std::endl;
        std::cout << "Entrez sur 'q' pour quitter." << std::endl;

        // Sauvegarde l'état actuel de la grille dans le fichier.
        saveCurrentState();
    }

    // Méthode pour sauvegarder l'état actuel de la grille dans un fichier.
    void saveCurrentState() {
        if (outputFile.is_open()) { 
            outputFile << "Itération: " << iterationCount << "\n"; 
            for (int x = 0; x < grid.getligne(); ++x) { 
                for (int y = 0; y < grid.getcolonne(); ++y) { 
                    outputFile << (grid.getCells()[x][y].getAlive() ? "1" : "0") << " ";

                }
                outputFile << "\n"; 
            }

            outputFile << "\n"; 
        }
    }

    // Méthode exécutée dans un thread séparé pour gérer les entrées utilisateur.
    void inputThread() {
        char input; 
        while (running) { // Boucle continue tant que le jeu est en cours.
            std::cin >> input; 
            if (input == 'q') { 
                running = false; 

            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');// Ignore le reste de la ligne pour éviter les problèmes d'entrée.
        }
    }


    // Méthode pour démarrer le jeu.
    void start() {
        // Lancement du thread pour gérer les entrées utilisateur.
        std::thread inputThread(&ConsoleGame::inputThread, this);
        while (running) { 

            displayGrid(); // Affiche la grille.
            grid.updateGrid(); // Met à jour l'état de la grille.
            iterationCount++; // Incrémente le compteur d'itérations.
            std::this_thread::sleep_for(std::chrono::milliseconds(delay)); // Pause entre les mises à jour.
        }

        inputThread.join(); // Attend que le thread d'entrée utilisateur se termine.
    }
};

